#!/usr/bin/env python3.1
import getopt, sys, os, socket, signal, logging, bz2, struct, ossaudiodev, traceback, time
# Written by MorgothV8: Lukasz Gryglicki, lukaszgryglicki@o2.pl, tel +48693582014
# globals
srv_sock=None
cli_sock=None
log_file='/tmp/PyVoice.log'
log_level=logging.INFO
glog=None
raw_audio_input='/dev/dsp'
raw_audio_output='/dev/dsp'
block_size=1024
use_oss_in=True
use_oss_out=True
n_read=0
n_sent=0
n_recieved=0
n_write=0
use_compress=True
dbg_mode=False
dbg_file_in='./recorded.raw'
dbg_file_out='./recieved.raw'
net_timeout=30
time_start=None
use_mixer=False
mixer_rec=90
mixer_mic=50
mixer_vol=40
mixer_pcm=40
#audio_fmt=ossaudiodev.AFMT_IMA_ADPCM
audio_fmt=ossaudiodev.AFMT_MU_LAW
#audio_fmt=ossaudiodev.AFMT_U8
#audio_fmt=ossaudiodev.AFMT_S16_LE
audio_chn=1
#audio_chn=2
#audio_spd=2000
#audio_spd=4000
#audio_spd=6000
audio_spd=8000
#audio_spd=11025
#audio_spd=22050
#audio_spd=44100

# main code starts
def main():
	global log_file
	global log_level
	global raw_audio_input
	global raw_audio_output
	global block_size
	global use_oss_in
	global use_oss_out
	global use_compress
	global audio_fmt
	global audio_chn
	global audio_spd
	global mixer_rec
	global mixer_mic
	global mixer_vol
	global mixer_pcm
	global use_mixer
	global dbg_mode

	try:
	    optList, args = getopt.getopt(sys.argv[1:], 'LH12345gmfceioyhxas:p:l:u:d:b:', \
		    ['help', 'accept', 'server=', 'port=', 'logfile=', 'input', 'output', \
		    'audio_in=', 'audio_out=', 'blocksize', 'rawin', 'rawout', 'rawall', \
		    'full', 'afmt=', 'achn=', 'aspd=', 'mixrec=', 'mixmic=', 'mixvol=', \
		    'mixpcm=', 'mix', 'debug', 'low', 'high'])
	except getopt.GetoptError as err:
		print(err)
		help_usage()
		sys.exit(1)
	port=9000
	server='localhost'
	listener=0
	Input=True
	DoubleMode=True
	for option, optVal in optList:
		if option == '--afmt':
			audio_fmt = int(optVal)
			print('Audio format:', audio_fmt)
		elif option == '--achn':
			audio_chn = int(optVal)
			print('Audio channels:', audio_chn)
		elif option == '--aspd':
			audio_spd = int(optVal)
			print('Audio speed:', audio_spd)
		elif option == '--mixrec':
			mixer_rec = int(optVal)
			print('Mixer REC:', mixer_rec)
		elif option == '--mixmic':
			mixer_mic = int(optVal)
			print('Mixer MIC:', mixer_mic)
		elif option == '--mixvol':
			mixer_vol = int(optVal)
			print('Mixer VOL:', mixer_vol)
		elif option == '--mixpcm':
			mixer_pcm = int(optVal)
			print('Mixer PCM:', mixer_pcm)
		elif option in ('-h', '--help'):
			help_usage()
			sys.exit(0)
		elif option in ('-L', '--low'):
			print('Lowest quality possible')
			audio_fmt=ossaudiodev.AFMT_U8
			audio_chn=1
			audio_spd=4096
			block_size=2048
		elif option in ('-H', '--high'):
			print('High quality mode')
			audio_fmt=ossaudiodev.AFMT_S16_LE
			audio_chn=1
			audio_spd=11050
		elif option in ('-m', '--mix'):
			use_mixer=True
			print('Using mixer settings')
		elif option in ('-g', '--debug'):
			dbg_mode=True
			print('Using debug file(s)')
		elif option in ('-p', '--port'):
			port=optVal
			print('Port: ', port)
		elif option in ('-s', '--server'):
			server=optVal
			print('Server: ', server)
		elif option in ('-u', '--audio_in'):
			raw_audio_input=optVal
			print('Audio: ', raw_audio_input)
		elif option in ('-d', '--audio_out'):
			raw_audio_output=optVal
			print('DSP: ', raw_audio_output)
		elif option in ('-a', '--accept'):
			print('Acceptor mode on')
			listener=1
		elif option in ('-l', '--logfile'):
			log_file=optVal
			print('Log: ', log_file)
		elif option in ('-y', '--full'):
			use_compress=False
			print('Uncompressed mode on')
		elif option in ('-c', '--rawin'):
			use_oss_in=False
			print('RAW input mode on')
		elif option in ('-e', '--rawout'):
			use_oss_out=False
			print('RAW output mode on')
		elif option in ('-f', '--rawall'):
			use_oss_in=False
			use_oss_out=False
			print('RAW all mode on')
		elif option in ('-b', '--blocksize'):
			block_size=int(optVal)
			print('BlockSize: ', block_size)
		elif option in ('-i', '--input'):
			Input=True
		elif option in ('-o', '--output'):
			Input=False
		elif option == '-x':
			print('Bidirection mode off')
			DoubleMode=False
		elif option == '-1':
			log_level=logging.DEBUG
		elif option == '-2':
			log_level=logging.INFO
		elif option == '-3':
			log_level=logging.WARNING
		elif option == '-4':
			log_level=logging.ERROR
		elif option == '-5':
			log_level=logging.CRITICAL
		else:
			print('Unknown option')
			help_usage()
			sys.exit(1)

	signal.signal(signal.SIGINT, sighandler)
	signal.signal(signal.SIGTERM, sighandler)
	signal.signal(signal.SIGHUP, sighandler)
	signal.siginterrupt(signal.SIGHUP, False)
	signal.signal(signal.SIGALRM, sighandler)

	if listener == 1:
		listener_deamon(port)
	else:
		if DoubleMode:
			client(server, port, True)
			client(server, port, False)
		else:
			client(server, port, Input)

	sys.exit(0)

def my_send(sock, bstr, n):
	nsend = 0
	signal.alarm(net_timeout)
	while nsend < n:
		nsend += sock.send(bstr[nsend:])
	
	signal.alarm(0)

def my_recv(sock, n):
	nrecv = 0
	bstr = b''
	signal.alarm(net_timeout)
	while nrecv < n:
		tmpstr = sock.recv(n-nrecv)
		nrecv += len(tmpstr)
		bstr += tmpstr
	signal.alarm(0)
	return bstr

def client(server, port, Input):
	global log_file
	global cli_sock

	try:
		pid = os.fork()
		if pid > 0:
			return
	except OSError as ex:
		print('fork() error: ', ex.errno, ex.strerror)
		glog.critical('fork() error: %d %s', ex.errno, ex.strerror)
		sys.exit(1)

	#print('Client connect to: ', server, port, ', input mode: ', Input)

	log_file+='.client'
	setup_logs()

	try:
		client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		client_socket.connect((server, port))
	except socket.error as ex:
		print('socket() error: ', ex)
		glog.critical('socket() error: %s', str(ex))
		sys.exit(1)
	#print('Connected to server')
	glog.info('Connected to %s:%d', server, port)
	cli_sock=client_socket
	if Input:
		glog.debug('Input mode client')
		my_send(client_socket, b'I', 1)
	else:
		glog.debug('Output mode client')
		my_send(client_socket, b'O', 1)
	my_recv(client_socket, 1)
	#print('Server says: ', str(data))

	if Input:
		input_sender(client_socket)
	else:
		my_send(client_socket, b'K', 1)
		input_reciever(client_socket)

def setup_mixer():
	if not use_mixer:
		glog.info('Skipping mixer settings')
		return

	scmd = 'mixer rec ' + str(mixer_rec) + ' > /dev/null'
	os.system( scmd )

	scmd = 'mixer mic ' + str(mixer_mic) + ' > /dev/null'
	os.system( scmd )

	scmd = 'mixer vol ' + str(mixer_vol) + ' > /dev/null'
	os.system( scmd )

	scmd = 'mixer pcm ' + str(mixer_pcm) + ' > /dev/null'
	os.system( scmd )


	glog.info('Mixer input set: rec=%d, mic=%d', mixer_rec, mixer_mic)
	glog.info('Mixer output set: vol=%d, pcm=%d', mixer_vol, mixer_pcm)


def input_sender(sock):
	global n_read
	global n_sent
	global use_compress
	global time_start

	try:
		if use_oss_in:
			glog.info('Using OSS driver for reading')
			audio = ossaudiodev.open('r')

			fmt = audio.setfmt( audio_fmt )
			chn = audio.channels( audio_chn )
			spd = audio.speed( audio_spd )

			glog.info('Opened OSS driver for: %s %s', audio.name, audio.mode)
			glog.info('Params %s %s %s, requested %s %s %s', fmt, chn, spd, audio_fmt, audio_chn, audio_spd)

			if fmt != audio_fmt or chn != audio_chn or spd != audio_spd:
				print('Failed to set audio parameters')
				glog.critical('Failed to set audio parameters')
				sock.close()
				sys.exit(1)

			#not used
			#mixer=ossaudiodev.openmixer()
			#mixer.set(ossaudiodev.SOUND_MIXER_REC, (100, 100))
			#mixer.set(ossaudiodev.SOUND_MIXER_MIC, (100, 100))
			#mixer.set_recsrc( 1 << ossaudiodev.SOUND_MIXER_MIC)
			#glog.info('Mixer configured')
		else:
			glog.info('Opening audio input: %s', raw_audio_input)
			audio = open(raw_audio_input, 'rb')
		#debug
		if dbg_mode:
			glog.info('Opening recording debug file: %s', dbg_file_in)
			outf = open(dbg_file_in, 'ab')

	except IOError as estr:

		print('Cannot open audio device: ', raw_audio_input, estr)
		glog.critical('Cannot open audio file: %s, %s', raw_audio_input, estr)
		glog.critical('Possible bad parameters requested: %s %s %s', audio_fmt, audio_chn, audio_spd)
		sock.close()
		sys.exit(1)

	#glog.info('Setup mixer mic rec=%d mic=%d, out vol=%d pcm=%d', mixer_rec, mixer_mic, mixer_vol, mixer_pcm)
	setup_mixer()
	time_start = time.time()
	glog.info('Begining audio transfer')
	nsteps = 0

	while True:
		try:
			adata = audio.read(block_size)
		except IOError as ex:
			glog.critical('IOError audio.read: %s', str(ex))
			sock.close()
			sys.exit(1)

		if not adata:
			glog.warning('EOF reached on input')
			blen = struct.pack('=L', 0)
			my_send(sock, blen, 4)
			my_recv(sock, 1)
			sock.close()
			sys.exit(1)

		glog.debug('Read block %d bytes', len(adata))
		if use_compress:
			cmpr = bz2.BZ2Compressor()
			cadata = cmpr.compress(adata)
			cadata += cmpr.flush()
			del cmpr
		else:
			cadata = adata

		blen = struct.pack('=L', len(cadata))

		try:
			my_send(sock, blen, 4)
			my_recv(sock, 1)
			my_send(sock, cadata, len(cadata))
			my_recv(sock, 1)
			n_read += len(adata)
			n_sent += len(cadata)
			nsteps += 1
			if (nsteps % 1000) == 0:
				traffic_stats()

			if dbg_mode:
				outf.write(adata)
			#print(len(cadata))

			glog.debug('Send %d bytes', len(cadata))

		except socket.error as ex:
			glog.critical('socket() error: %s', str(ex))
			sock.close()
			sys.exit(1)

def handle_client(s):
	try:
		pid = os.fork()
		if pid > 0:
			return True
	except OSError as ex:

		glog.critical('fork() error: %d %s', ex.errno, ex.strerror)
		sys.exit(1)

	cli_type = str(my_recv(s, 1))
	#print('cli_type', cli_type)
	glog.info('Client type is %s', cli_type)

	if cli_type == str(b'I'):
		glog.debug('Input client')
		Input=True
	elif cli_type == str(b'O'):
		glog.debug('Output client')
		Input=False
	else:
		glog.error('Unknown client type: %s', cli_type)
		#my_send(s, b'!', 1)
		return False

	glog.debug('Input: %s', Input)
	my_send(s, b'K', 1)

	if Input:
		glog.info('Server in recieve mode')
		input_reciever(s)
	else:
		glog.info('Server in send mode')
		my_recv(s, 1)
		input_sender(s)

	glog.info('Client finished')
	sys.exit(0)

def input_reciever(sock):
	global n_write
	global n_recieved
	global use_compress
	global time_start

	try:
		if use_oss_out:
			glog.info('Using OSS driver for writing')
			dsp = ossaudiodev.open('w')

			fmt = dsp.setfmt( audio_fmt )
			chn = dsp.channels( audio_chn )
			spd = dsp.speed( audio_spd )

			glog.info('Opened OSS driver for: %s %s', dsp.name, dsp.mode)
			glog.info('Params %s %s %s, requested %s %s %s', fmt, chn, spd, audio_fmt, audio_chn, audio_spd)

			if fmt != audio_fmt or chn != audio_chn or spd != audio_spd:
				print('Failed to set audio parameters')
				glog.critical('Failed to set audio parameters')
				sock.close()
				sys.exit(1)

		else:
			glog.info('Opening DSP file: %s', raw_audio_output)
			dsp = open(raw_audio_output, 'wb')

		if dbg_mode:
			glog.info('Opening play debug file: %s', dbg_file_out)
			outf = open(dbg_file_out, 'ab')

	except IOError as estr:
		glog.critical('Cannot open dsp file: %s, %s', raw_audio_output, estr)
		glog.critical('Possible bad parameters requested: %s %s %s', audio_fmt, audio_chn, audio_spd)
		sock.close()
		sys.exit(1)

	#glog.info('Setup mixer mic rec=%d mic=%d, out vol=%d pcm=%d', mixer_rec, mixer_mic, mixer_vol, mixer_pcm)
	setup_mixer()
	time_start = time.time()
	glog.info('Start recieving of audio')
	nsteps = 0

	while True:
		try:
			blen = my_recv(sock, 4)
			if not blen:
				glog.warning('Client finished')
				sock.close()
				sys.exit(0)
			l = struct.unpack('=L', blen)
			#print(l[0])
			my_send(sock, b'>', 1)
			bufsize = l[0]
			bbytes = my_recv(sock, bufsize)
			glog.debug('Length %d, Read %d', l[0], len(bbytes))

			if len(bbytes) < 4:
				glog.warning('Client finished')
				sock.close()
				sys.exit(0)

			#if use_compress and bbytes[0] != 66:
			#	bbytes = b'B' + bbytes
			#	glog.warning('Changed bytes: %s', str(bbytes[:10]))

			#print(len(bbytes))
			my_send(sock, b'>', 1)

		except socket.error as ex:
			glog.critical('socket() error: %s', str(ex))
			sock.close()
			sys.exit(1)

		except struct.error as ex:
			glog.critical('unpack() error: %s', str(ex))
			sock.close()
			sys.exit(1)

		if len(bbytes) > 0:
			if use_compress:
				dbytes=''
				cmpr = bz2.BZ2Decompressor()
				try:
					dbytes = cmpr.decompress(bbytes)
					del cmpr
				except IOError as estr:
					#print('Decompression error:', estr)
					glog.warning('Decompression error: %s', estr)
				glog.debug('Decompressed %d bytes', len(dbytes))
			else:
				dbytes = bbytes
				glog.debug('Recieved %d bytes', len(dbytes))

			try:
				dsp.write(dbytes)
			except IOError as ex:
				glog.critical('IOError: dsp.write: %s', str(ex))
				sock.close()
				sys.exit(1)

			n_recieved += len(bbytes)
			n_write += len(dbytes)
			nsteps += 1
			if (nsteps % 1000) == 0:
				traffic_stats()

			if dbg_mode:
				outf.write(dbytes)

def listener_deamon(port):
	global srv_sock
	global log_file

	log_file+='.server'
	setup_logs()

	try:
		pid = os.fork()
		if pid > 0:
			return
	except OSError as ex:
		print('fork() error: ', ex.errno, ex.strerror)
		glog.critical('fork() error: %d %s', ex.errno, ex.strerror)
		sys.exit(1)
	try:
		server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		server_socket.bind(('',port))
		server_socket.listen(5)
	except socket.error as ex:
		print('socket() error: ', ex)
		glog.critical('socket() error: %s', str(ex))
		sys.exit(1)
	running = True
	srv_sock = server_socket
	while running:
		try:
			glog.debug('Wait for connection...')
			s, address = server_socket.accept()
			glog.info('Got client %s', str(address))
			handle_client(s)
		except socket.error as ex:
			#print('socket() error: ', ex)
			glog.error('socket() error: %s', str(ex))

def traffic_stats():

	if not time_start:
		return
	
	time_end = time.time()
	tdiff = time_end - time_start
	glog.info('Elapsed time %f', tdiff)

	if n_read > 0 and n_sent > 0:
		glog.info('Bytes read %d', n_read)
		bps = n_read / tdiff
		glog.info('BPS uncompressed %f', bps)

		glog.info('Bytes sent %d', n_sent)
		bps = n_sent / tdiff
		glog.info('BPS %f', bps)

		bps = (n_sent * 100.0) / n_read
		glog.info('Compression %f', bps)

	if n_write > 0 and n_recieved > 0:
		glog.info('Bytes recieved %d', n_recieved)
		bps = n_recieved / tdiff
		glog.info('BPS %f', bps)

		glog.info('Bytes written %d', n_write)
		bps = n_write / tdiff
		glog.info('BPS uncompressed %f', bps)

		bps = (n_recieved * 100.0) / n_write
		glog.info('Compression %f', bps)

	if n_sent > 0 and n_recieved > 0:
		bps = (n_sent + n_recieved) / tdiff
		glog.info('Traffic BPS: %f', bps)

def sighandler(signum, frame):

	if signum == signal.SIGHUP:
		glog.warning('Got signal %d', signum)
		traffic_stats()
		return

	glog.warning('Got signal %d quiting', signum)
	traffic_stats()

	if signum == signal.SIGALRM:
		glog.critical('Current operation timeout, exiting')
		#glog.critical('Frame: %s(): %s', frame.f_code.co_name, frame.f_lineno)
		for sfile, sline, sfunc, sdummy in traceback.extract_stack(frame):
		    glog.critical( 'Traceback: File: Line: %s, %s Function: %s', sfile, sline, sfunc)

	if srv_sock:
		glog.debug('Closing server socket')
		srv_sock.close()

	if cli_sock:
		glog.debug('Closing client socket')
		cli_sock.close()

	sys.exit(0)

def setup_logs():
	global log_file
	global log_level
	global glog

	logger = logging.getLogger('PyVoice')
	logger.setLevel(log_level)
	ch = logging.FileHandler(log_file, mode='a')
	fmt = logging.Formatter('%(asctime)s: %(levelname)s: %(message)s')
	ch.setFormatter(fmt)
	logger.addHandler(ch)

	#logging.basicConfig(filename=log_file, level=log_level)
	logger.info('Otwarto log: %s', log_file)
	glog = logger
	
def help_usage():
	print(sys.argv[0], '[-h|--help] -u|--audio file_dsp_in -d|--dsp file_dsp_out -a|--accept -i|--input -o|--output -s|--server serwername -p|--port portnum -l|--logfile logfn -1-5 -b|--blocksize block -x -c|--rawin -e|--rawout -f|--rawall -y|--full --afmt num --achn num --aspd num --mixrec 0-100 --mixmic 0-100 --mixvol 0-100 --mixpcm 0-100 -n|--mix -g|--debug -L|--low -H|--high')
	print('Default server is localhost')
	print('Default port is 9000')
	print('Acceptor(server) mode is -a')
	print('Log file name with -l, level -1-5') 
	print('Input client is recording and playing on server (default)')
	print('Output client is playing recieved from server')
	print('Block size set -b, --blocksize')
	print('Bidirection mode set off -x, use -i or -o to select mode')
	print('Raw audio input device set -u, --audio_in')
	print('Raw audio output device set -d, --audio_out')
	print('Raw input mode set -c, --rawin')
	print('Raw output mode set -e, --rawout')
	print('Raw all mode set -f, --rawall or -c -e')
	print('Uncompressed mode set -y, --full')
	print('In OSS mode (default), audio formats: --afmt, --achn, --aspd')
	print('Audio format --afmt: 1=MULAW, 2=ALAW, 4=IMADPCM, 8=U8, 16=S16LE, 32=S16BE, 64=S8, 128=U16LE, 256=U16BE')
	print('Audio channels --achn: 1, 2, ?')
	print('Audio speed --aspd: 8000, 11025, 22050, 44100, 96000')
	print('Audio defaults: fmt=MULAW, chn=1, spd=8000')
	print('Mixer input --mixrec --mixmic in range 0-100, default 90, 50')
	print('Mixer output --mixvol --mixpcm in range 0-100, default 40, 40')
	print('Use mixer settings -m, --mix')
	print('Debug write in/out files use -g, --debug')
	print('Lowest possible quality (deformed voice) --low, -L')
	print('Quite high quality --high, -H')

if __name__ == '__main__':
    main()

