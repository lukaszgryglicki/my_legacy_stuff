#sox -t null /dev/null -r 96000 -b 16 -c 1 sine.wav synth 10.0 sine 80.0 sine 20050
sox -t null /dev/null -r 96000 -b 16 -c 1 $1 synth $2 sine $3
