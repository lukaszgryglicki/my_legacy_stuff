#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

struct MyRetText
{
    GtkWidget *dlg, *ent;
    char* ptr;
};

struct cv_cfg
{
    void clear()
    {
    	myid = targetid = 0;
	cache_size = 3200;	// 640, 800, 1120, 1600, 2400 160=0.02s
	debug = 0;
	quality = 1;
	complexity = 10;
	vbr = 1;
	vad = 1;
	dtx = 1;
	timeout = -30;
	strcpy(inum, "/dev/dsp");
	strcpy(onum, "/dev/dsp");
	exmode = 0;
	srv_sock = cli_sock = 0;
	wide = 1;
	enh_r = 1.0;
	enh_w = 1.0;
	save = 0;
	n_sync = 50;
	hz = 8000;
    }

    cv_cfg()
    {
	inum = new char[1024];
	onum = new char[1024];

	clear();
    }

    ~cv_cfg()
    {
	delete [] inum;
	delete [] onum;
    }

    int myid, targetid, cache_size, debug, quality;
    int timeout, exmode, complexity, vbr, vad, dtx;
    int srv_sock, cli_sock, wide;
    int save, n_sync, hz;
    char *inum, *onum;
    float enh_r, enh_w;
};

cv_cfg cfg;
GtkWidget* glob_l;
char glob_last_cfg[1024];
int pidlog, port;
char ip[20];

bool check_config(char* err)
{
	if (cfg.myid > 0 && cfg.targetid > 0) cfg.exmode = 1;
	else cfg.exmode = 0;
	
	if (cfg.exmode && (cfg.myid < 1 || cfg.targetid < 1 || cfg.myid == cfg.targetid))
	{
	    sprintf(err, "Bad extened mode IDs: %d %d", cfg.myid, cfg.targetid);
	    return false;
	}

	if (!cfg.exmode && (cfg.myid > 0 || cfg.targetid > 0))
	{
	    sprintf(err, "If you want to use ext_mode, you have to select both myid and targetid");
	    return false;
	}

	if (cfg.cache_size < 0x40 || cfg.cache_size > 0x8000)
	{
	    sprintf(err, "Network cache too low or too high");
	    return false;
	}

	if (cfg.quality < 0 || cfg.quality > 10)
	{
	    sprintf(err, "Speex quality must be in range of 0-10");
	    return false;
	}

	if (cfg.timeout > 0x8000 || cfg.timeout < -0x8000)
	{
	    sprintf(err, "Network timeout to high (absolute value)");
	    return false;
	}

	if (cfg.enh_r < 0.01 || cfg.enh_r > 10.) 
	{
	    sprintf(err, "Enchance input not in range 0.01-10");
	    return false;
	}

	if (cfg.enh_w < 0.01 || cfg.enh_w > 10.) 
	{
	    sprintf(err, "Enchance output not in range 0.01-10");
	    return false;
	}

	if (cfg.complexity < 0 || cfg.complexity > 10) 
	{
	    sprintf(err, "Complexity must be in range 0-10");
	    return false;
	}

	if (cfg.n_sync < 1 || cfg.n_sync > 1000) 
	{
	    sprintf(err, "Sync communication must be in range 1-1000");
	    return false;
	}

	if (cfg.hz < 2000 || cfg.hz > 24000) 
	{
	    sprintf(err, "Bad samplerate value, allowed 2kHz - 24 kHz");
	    return false;
	}

	return true;
}

void btn_server(GtkWidget* w, gpointer l)
{
    char errstr[256];

    if (!check_config(errstr))
    {
	gtk_label_set_text( GTK_LABEL( l ), errstr );
	return;
    }

    char* cmd = new char[2048];

    sprintf(cmd, "cvoice -a -I %s -O %s -c %d %s -q %d -C %d -T %d %s -R %.3f -W %.3f %s -y %d -H %d -p %d %s"
	    , cfg.inum, cfg.onum, cfg.cache_size / 160
	    , cfg.debug ? "-d" : "", cfg.quality, cfg.complexity, cfg.timeout
	    , cfg.wide ? "" : "-8", cfg.enh_r, cfg.enh_w, cfg.save ? "-S" : "", cfg.n_sync, cfg.hz 
	    , port, pidlog ? "-P" : "");

    printf("CMD: %s\n", cmd);
    system( cmd );

    gtk_label_set_text( GTK_LABEL( l ), "Starting server" );

    delete [] cmd;
}

void btn_client(GtkWidget* w, gpointer l)
{
    char errstr[256];

    if (!check_config(errstr))
    {
	gtk_label_set_text( GTK_LABEL( l ), errstr );
	return;
    }

    char* cmd = new char[2048];

    sprintf(cmd, "cvoice -I %s -O %s -c %d %s -q %d -C %d -T %d %s -R %.3f -W %.3f %s -y %d -H %d -i %s -p %d %s"
	    , cfg.inum, cfg.onum, cfg.cache_size / 160
	    , cfg.debug ? "-d" : "", cfg.quality, cfg.complexity, cfg.timeout
	    , cfg.wide ? "" : "-8", cfg.enh_r, cfg.enh_w, cfg.save ? "-S" : "", cfg.n_sync, cfg.hz
	    , ip, port, pidlog ? "-P" : "");

    printf("CMD: %s\n", cmd);
    system( cmd );

    gtk_label_set_text( GTK_LABEL( l ), "Connecting to server" );

    delete [] cmd;
}

void btn_stop(GtkWidget* w, gpointer l)
{
    system("killall cvoice");
    system("killall -9 cvoice");
    gtk_label_set_text( GTK_LABEL( l ), "Stopped cvoice" );
    printf("Stopped.\n");
}

void my_return_txt( GtkWidget* w, gpointer p)
{
    MyRetText* myret;

    myret = (MyRetText*)p;

    //printf("SCOPY '%s' %p %p\n", myret->ptr, myret->dlg, myret->ent);
    strcpy(myret->ptr, gtk_entry_get_text( GTK_ENTRY( myret->ent  ) ) );
    //printf("SDONE '%s' %p %p\n", myret->ptr, myret->dlg, myret->ent);
    gtk_widget_destroy( myret->dlg );

    delete myret;
}

void get_user_input(const char* question, char* ptr, const char* def)
{
    MyRetText* myret;
    GtkWidget *dlg, *o, *a, *e, *t;

    strcpy(ptr, "");
    dlg = gtk_dialog_new();

    gtk_window_set_title( GTK_WINDOW( dlg ), question);
    gtk_window_set_default_size( GTK_WINDOW( dlg ), 400, 60);

    t = gtk_table_new(2, 3, TRUE);
    gtk_table_set_row_spacings( GTK_TABLE( t ), 2 );
    gtk_table_set_col_spacings( GTK_TABLE( t ), 2 );

    a = gtk_dialog_get_content_area( GTK_DIALOG( dlg ) );

    e = gtk_entry_new();
    if (strcmp(def, "")) gtk_entry_set_text( GTK_ENTRY( e ), def);

    o = gtk_button_new_with_label( "OK" );

    gtk_table_attach_defaults( GTK_TABLE( t ), e, 0, 3, 0, 1);
    gtk_table_attach_defaults( GTK_TABLE( t ), o, 1, 2, 1, 2);

    gtk_container_add(GTK_CONTAINER( a ), t );

    myret = new MyRetText;

    myret->ptr = ptr;
    myret->dlg = dlg;
    myret->ent = e;

    //printf("START '%s' %p %p\n", myret->ptr, myret->dlg, myret->ent);

    g_signal_connect( o, "clicked", G_CALLBACK( my_return_txt ), ( gpointer )myret );
    g_signal_connect_swapped( dlg, "response", G_CALLBACK( gtk_widget_destroy ), dlg );

    gtk_widget_show_all( dlg );
    gtk_dialog_run( GTK_DIALOG( dlg ) );
    //gtk_widget_destroy( dlg );

    //printf("END '%s'\n", ptr);
}

void save_config(FILE* fp, cv_cfg& c)
{
	fprintf(fp, "Input=%s\n", c.inum);
	fprintf(fp, "Output=%s\n", c.onum);
	fprintf(fp, "CacheSize=%d\n", c.cache_size);
	fprintf(fp, "Debug=%d\n", c.debug);
	fprintf(fp, "Quality=%d\n", c.quality);
	fprintf(fp, "Complexity=%d\n", c.complexity);
	fprintf(fp, "Timeout=%d\n", c.timeout);
	fprintf(fp, "Wide=%d\n", c.wide);
	fprintf(fp, "EnchanceR=%.3f\n", c.enh_r);
	fprintf(fp, "EnchanceW=%.3f\n", c.enh_w);
	fprintf(fp, "Save=%d\n", c.save);
	fprintf(fp, "NSync=%d\n", c.n_sync);
	fprintf(fp, "HZ=%d\n", c.hz);
	fprintf(fp, "PIDlogs=%d\n", pidlog);
	fprintf(fp, "Port=%d\n", port);
	fprintf(fp, "IP=%s\n", ip);
}

void load_config(FILE* fp, cv_cfg& c)
{
	fscanf(fp, "Input=%s\n", c.inum);
	fscanf(fp, "Output=%s\n", c.onum);
	fscanf(fp, "CacheSize=%d\n", &c.cache_size);
	fscanf(fp, "Debug=%d\n", &c.debug);
	fscanf(fp, "Quality=%d\n", &c.quality);
	fscanf(fp, "Complexity=%d\n", &c.complexity);
	fscanf(fp, "Timeout=%d\n", &c.timeout);
	fscanf(fp, "Wide=%d\n", &c.wide);
	fscanf(fp, "EnchanceR=%f\n", &c.enh_r);
	fscanf(fp, "EnchanceW=%f\n", &c.enh_w);
	fscanf(fp, "Save=%d\n", &c.save);
	fscanf(fp, "NSync=%d\n", &c.n_sync);
	fscanf(fp, "HZ=%d\n", &c.hz);
	fscanf(fp, "PIDlogs=%d\n", &pidlog);
	fscanf(fp, "Port=%d\n", &port);
	fscanf(fp, "IP=%s\n", ip);
}

int get_user_yesno(const char* q)
{
    GtkWidget* dlg;
    gint res;

    dlg = gtk_message_dialog_new( NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, q );
    gtk_window_set_title( GTK_WINDOW( dlg ), "Question");
    res = gtk_dialog_run( GTK_DIALOG( dlg ) );

    gtk_widget_destroy( dlg );

    if (res == GTK_RESPONSE_YES) 
    {
	return 1;
    }
    else
    {
	return 0;
    }
}

void user_info_msg(const char* m)
{
    GtkWidget* dlg;

    dlg = gtk_message_dialog_new( NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, m );
    gtk_window_set_title( GTK_WINDOW( dlg ), "Information");
    gtk_dialog_run( GTK_DIALOG( dlg ) );

    gtk_widget_destroy( dlg );
}

void btn_cfg_display(GtkWidget* _w, gpointer _l)
{
    	char fp[256], msg[2048];

    	gtk_label_set_text( GTK_LABEL( _l ), "Display configuration" );

	strcpy(msg, "");

	sprintf(fp, "IP %s\n", ip);
	strcat(msg, fp);

	sprintf(fp, "Port %d\n", port);
	strcat(msg, fp);

	sprintf(fp, "Input device %s\n", cfg.inum);
	strcat(msg, fp);

	sprintf(fp, "Output device %s\n", cfg.onum);
	strcat(msg, fp);

	sprintf(fp, "Cache size %d (%d packets)\n", cfg.cache_size, cfg.cache_size / 160);
	strcat(msg, fp);

	if (cfg.debug > 0)
	{
		sprintf(fp, "Debug on\n");
		strcat(msg, fp);
	}

	if (pidlog > 0)
	{
		sprintf(fp, "Using PIDs in log names\n");
		strcat(msg, fp);
	}

	sprintf(fp, "Speex quality %d\n", cfg.quality);
	strcat(msg, fp);

	sprintf(fp, "Encoding algorithm complexity %d\n", cfg.complexity);
	strcat(msg, fp);

	sprintf(fp, "Opereration timeout %d%s\n", abs( cfg.timeout ), cfg.timeout < 0 ? " after connect" : "" );
	strcat(msg, fp);

	if (cfg.wide)
	{
		sprintf(fp, "16 bit sound mode\n");
	}
	else
	{
		sprintf(fp, "8 bit sound mode\n");
	}
	strcat(msg, fp);

	sprintf(fp, "Enchance input %.3f\n", cfg.enh_r);
	strcat(msg, fp);

	sprintf(fp, "Enchance output %.3f\n", cfg.enh_w);
	strcat(msg, fp);

	if (cfg.save)
	{
		sprintf(fp, "Recording sound data\n");
		strcat(msg, fp);
	}

	sprintf(fp, "Synchronize buffers after %d cycles\n", cfg.n_sync);
	strcat(msg, fp);

	sprintf(fp, "Sample rate in Hz %d\n", cfg.hz);
	strcat(msg, fp);

	user_info_msg( msg );
}

void btn_cfg_save(GtkWidget* _w, gpointer _l)
{
    FILE* fp;
    char err[256], fn[512], q[32];

    if (!check_config(err))
    {
    	gtk_label_set_text( GTK_LABEL( _l ), err );
	return;
    }

    strcpy(q, "Save to file: ");
    get_user_input( q, fn, glob_last_cfg );

    if (!strcmp(fn, ""))
    {
    	gtk_label_set_text( GTK_LABEL( _l ), "No file name given" );
	return ;
    }

    int ov = 1;
    fp = fopen(fn, "r");
    if (fp)
    {
	fclose( fp );
	strcpy(q, "File exists, overwrite?");
	ov = get_user_yesno( q );
    }

    if (!ov)
    {
    	gtk_label_set_text( GTK_LABEL( _l ), "Not overwritten" );
	return ;
    }

    fp = fopen(fn, "w");
    if (!fp)
    {
	sprintf(err, "Cannot save to: %s", fn);
    	gtk_label_set_text( GTK_LABEL( _l ), err );
	return;
    }

    save_config( fp, cfg );

    fclose( fp );

    strcpy(glob_last_cfg, fn );

    sprintf(err, "Saved to: %s", fn);
    gtk_label_set_text( GTK_LABEL( _l ), err );
}

void btn_cfg_load(GtkWidget* _w, gpointer _l)
{
    FILE* fp;
    char err[256], fn[512], q[32];

    strcpy(q, "Load from file: ");
    get_user_input( q, fn, glob_last_cfg );

    if (!strcmp(fn, ""))
    {
    	gtk_label_set_text( GTK_LABEL( _l ), "No file name given" );
	return ;
    }

    fp = fopen(fn, "r");
    if (!fp)
    {
	sprintf(err, "Cannot load from: %s", fn);
    	gtk_label_set_text( GTK_LABEL( _l ), err );
	return;
    }

    load_config( fp, cfg );

    fclose( fp );

    if (!check_config(err))
    {
    	gtk_label_set_text( GTK_LABEL( _l ), err );
	return;
    }

    strcpy(glob_last_cfg, fn );

    sprintf(err, "Loaded from: %s", fn);
    gtk_label_set_text( GTK_LABEL( _l ), err );
}

void btn_cfg_clear(GtkWidget* _w, gpointer _l)
{
    cfg.clear();
    gtk_label_set_text( GTK_LABEL( _l ), "Defaults loaded" );
}

void btn_cfg_toggleopt(GtkWidget* _w, gpointer i)
{
    char tmp[256];
    // Toggles: Debug, Wide, Save
    if (i == (gpointer)0)	// debug
    {
	cfg.debug = !cfg.debug;
	if (cfg.debug) strcpy(tmp, "Debug mode on");
	else strcpy(tmp, "Debug mode off");
    }
    else if (i == (gpointer)1)	// wide mode
    {
	cfg.wide = !cfg.wide;
	if (cfg.wide) strcpy(tmp, "16 bit sound mode");
	else strcpy(tmp, "8 bit sound mode");
    }
    else if (i == (gpointer)2)	// save
    {
	cfg.save = !cfg.save;
	if (cfg.save) strcpy(tmp, "Dumping sound data to MSPX files");
	else strcpy(tmp, "Sound dumping is off");
    }
    else if (i == (gpointer)3)	// PID logs
    {
	pidlog = !pidlog;
	if (pidlog) strcpy(tmp, "Using PIDs in log file names");
	else strcpy(tmp, "Not using PIDs in log file names");
    }
	
    gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
}

void btn_cfg_ip(GtkWidget* _w, gpointer _l)
{
    	char tmp[1024], def[32], q[64];
	int iopt[4], i;

	sprintf(tmp, "IP numer config, current: %s", ip);
	gtk_label_set_text( GTK_LABEL( glob_l ), tmp );

	strcpy(q, "IP number: ");
	sprintf(def, "%s", ip);
    	get_user_input( q, tmp, def );

	if (!strcmp(tmp, "") || sscanf(tmp, "%d.%d.%d.%d", &iopt[0], &iopt[1], &iopt[2], &iopt[3]) < 4)
	{
	    sprintf(tmp, "No new IP set, using previous: %s", ip);
	    gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
	    return;
	}

	for (i=0;i<4;i++)
	{
		if (iopt[i] < 0 || iopt[i] > 255)
		{
	    		sprintf(tmp, "No new IP set, using previous: %s", ip);
	    		gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
	    		return;
		}
	}

	strcpy(ip, tmp);
    	sprintf(tmp, "New IP: %s", ip);
	gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
}

void btn_cfg_intopt(GtkWidget* _w, gpointer i)
{
    char tmp[1024], q[64], def[32];
    int iopt;
    float fopt;
    // 0 - cache,   1 - quality, 2 - complexity
    // 3 - EnhR,    4 - EnhW,    5 - Sample rate
    // 6 - synchro, 7 - timeout, 8 - port

    if (i == (gpointer)0)	// cache
    {
	sprintf(tmp, "Cache config, current: %d bytes (%d packets)", cfg.cache_size, cfg.cache_size / 160);
	gtk_label_set_text( GTK_LABEL( glob_l ), tmp );

	strcpy(q, "Cache size in 160b packets (1-200):");
	sprintf(def, "%d", cfg.cache_size / 160);
    	get_user_input( q, tmp, def );

	iopt = 0;
	if (!strcmp(tmp, "") || sscanf(tmp, "%d", &iopt) < 1)
	{
	    sprintf(tmp, "No new cache set, using previous: %d", cfg.cache_size / 160);
	    gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
	    return;
	}

	if (iopt < 1 || iopt > 200)
	{
	    sprintf(tmp, "Bad cache size: %d, using previous: %d", iopt, cfg.cache_size / 160);
	    gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
	    return;
	}

	cfg.cache_size = iopt * 160;
    	sprintf(tmp, "New cache size %d bytes (%d packets)", cfg.cache_size, cfg.cache_size / 160);
	gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
    }
    else if (i == (gpointer)1)	// quality
    {
	sprintf(tmp, "Speex quality config, current: %d", cfg.quality);
	gtk_label_set_text( GTK_LABEL( glob_l ), tmp );

	strcpy(q, "Speex quality (0-10):");
	sprintf(def, "%d", cfg.quality);
    	get_user_input( q, tmp, def );

	iopt = 0;
	if (!strcmp(tmp, "") || sscanf(tmp, "%d", &iopt) < 1)
	{
	    sprintf(tmp, "No new quality set, using previous: %d", cfg.quality);
	    gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
	    return;
	}

	if (iopt < 0 || iopt > 10)
	{
	    sprintf(tmp, "Bad quality value: %d, using previous: %d", iopt, cfg.quality);
	    gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
	    return;
	}

	cfg.quality = iopt;
    	sprintf(tmp, "New speex quality: %d", cfg.quality);
	gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
    }
    else if (i == (gpointer)2)	// complexity
    {
	sprintf(tmp, "Speex complexity config, current: %d", cfg.complexity);
	gtk_label_set_text( GTK_LABEL( glob_l ), tmp );

	strcpy(q, "Speex complexity (0-10):");
	sprintf(def, "%d", cfg.complexity);
    	get_user_input( q, tmp, def );

	iopt = 0;
	if (!strcmp(tmp, "") || sscanf(tmp, "%d", &iopt) < 1)
	{
	    sprintf(tmp, "No new complexity set, using previous: %d", cfg.complexity);
	    gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
	    return;
	}

	if (iopt < 0 || iopt > 10)
	{
	    sprintf(tmp, "Bad complexity value: %d, using previous: %d", iopt, cfg.complexity);
	    gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
	    return;
	}

	cfg.complexity = iopt;
    	sprintf(tmp, "New speex complexity: %d", cfg.complexity);
	gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
    }
    else if (i == (gpointer)3)	// Enchance Input
    {
	sprintf(tmp, "Config software input enchance, now: %.0f%%", cfg.enh_r * 100.);
	gtk_label_set_text( GTK_LABEL( glob_l ), tmp );

	strcpy(q, "Software input enchance (100% is default):");
	sprintf(def, "%.0f", cfg.enh_r * 100.);
    	get_user_input( q, tmp, def );

	fopt = 0.;
	if (!strcmp(tmp, "") || sscanf(tmp, "%f", &fopt) < 1)
	{
	    sprintf(tmp, "No new software input enchance set, using previous: %.0f%%", cfg.enh_r * 100.);
	    gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
	    return;
	}

	if (fopt < 1. || fopt > 1000.)
	{
	    sprintf(tmp, "Bad enchance input value: %.0f, using previous: %.0f%%", fopt, cfg.enh_r * 100.);
	    gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
	    return;
	}

	cfg.enh_r = fopt / 100.;
    	sprintf(tmp, "New enchance input: %.0f%%", cfg.enh_r * 100.);
	gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
    }
    else if (i == (gpointer)4)	// Enchance Output
    {
	sprintf(tmp, "Config software output enchance, now: %.0f%%", cfg.enh_w * 100.);
	gtk_label_set_text( GTK_LABEL( glob_l ), tmp );

	strcpy(q, "Software output enchance (100% is default):");
	sprintf(def, "%.0f", cfg.enh_w * 100.);
    	get_user_input( q, tmp, def );

	fopt = 0.;
	if (!strcmp(tmp, "") || sscanf(tmp, "%f", &fopt) < 1)
	{
	    sprintf(tmp, "No new software output enchance set, using previous: %.0f%%", cfg.enh_w * 100.);
	    gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
	    return;
	}

	if (fopt < 1. || fopt > 1000.)
	{
	    sprintf(tmp, "Bad enchance output value: %.0f, using previous: %.0f%%", fopt, cfg.enh_w * 100.);
	    gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
	    return;
	}

	cfg.enh_w = fopt / 100.;
    	sprintf(tmp, "New enchance output: %.0f%%", cfg.enh_w * 100.);
	gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
    }
    else if (i == (gpointer)5)	// Sample rate
    {
	sprintf(tmp, "Config sample rate, current: %d Hz", cfg.hz);
	gtk_label_set_text( GTK_LABEL( glob_l ), tmp );

	strcpy(q, "Sample rate (2000-24000):");
	sprintf(def, "%d", cfg.hz);
    	get_user_input( q, tmp, def );

	iopt = 0;
	if (!strcmp(tmp, "") || sscanf(tmp, "%d", &iopt) < 1)
	{
	    sprintf(tmp, "No new sample rate set, using previous: %d", cfg.hz);
	    gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
	    return;
	}

	if (iopt < 2000 || iopt > 24000)
	{
	    sprintf(tmp, "Bad sample rate value: %d, using previous: %d", iopt, cfg.hz);
	    gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
	    return;
	}

	cfg.hz = iopt;
    	sprintf(tmp, "New sample rate: %d Hz", cfg.hz);
	gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
    }
    else if (i == (gpointer)6)	// n_sync
    {
	sprintf(tmp, "Config synchronize, current: after %d packets", cfg.n_sync);
	gtk_label_set_text( GTK_LABEL( glob_l ), tmp );

	strcpy(q, "Synchronize after packets:");
	sprintf(def, "%d", cfg.n_sync);
    	get_user_input( q, tmp, def );

	iopt = 0;
	if (!strcmp(tmp, "") || sscanf(tmp, "%d", &iopt) < 1)
	{
	    sprintf(tmp, "No new synchronize set, using previous: %d", cfg.n_sync);
	    gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
	    return;
	}

	if (iopt < 0 || iopt > 2000000000)
	{
	    sprintf(tmp, "Bad synchronize: %d, using previous: %d", iopt, cfg.n_sync);
	    gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
	    return;
	}

	cfg.n_sync = iopt;
    	sprintf(tmp, "New synchonize option: after %d packets", cfg.n_sync);
	gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
    }
    else if (i == (gpointer)7)	// timeout
    {
	sprintf(tmp, "Config timeout, current: %ds%s", abs( cfg.timeout), cfg.timeout < 0 ? " after connect" : "");
	gtk_label_set_text( GTK_LABEL( glob_l ), tmp );

	strcpy(q, "Timeout (negative for timeout after connect):");
	sprintf(def, "%d", cfg.timeout);
    	get_user_input( q, tmp, def );

	iopt = 0;
	if (!strcmp(tmp, "") || sscanf(tmp, "%d", &iopt) < 1)
	{
	    sprintf(tmp, "No new timeout set, using previous: %d", cfg.timeout);
	    gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
	    return;
	}

	if (abs(iopt) > 864000)	// dekada
	{
	    sprintf(tmp, "Bad timeout: %d, using previous: %d", iopt, cfg.timeout);
	    gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
	    return;
	}

	cfg.timeout = iopt;
    	sprintf(tmp, "New timeout: %ds%s", abs(cfg.timeout), cfg.timeout < 0 ? " after connect" : "");
	gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
    }
    else if (i == (gpointer)8)	// port
    {
	sprintf(tmp, "Config port, current: %d", port);
	gtk_label_set_text( GTK_LABEL( glob_l ), tmp );

	strcpy(q, "Port number:");
	sprintf(def, "%d", port);
    	get_user_input( q, tmp, def );

	iopt = 0;
	if (!strcmp(tmp, "") || sscanf(tmp, "%d", &iopt) < 1)
	{
	    sprintf(tmp, "No new port set, using previous: %d", port);
	    gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
	    return;
	}

	if (iopt < 1024 || iopt > 65535)
	{
	    sprintf(tmp, "Bad port number: %d, using previous: %d", iopt, port);
	    gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
	    return;
	}

	port = iopt;
    	sprintf(tmp, "New port: %d", port);
	gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
    }
}

void btn_cfg_iofile(GtkWidget* _w, gpointer i)
{
    char q[64];
    char *ptr;
    char tmp[1024];
    char mode[16];

    if (i == (gpointer)0)
    {
	ptr = cfg.inum;
	sprintf(tmp, "Input device config, current: %s", ptr);
	gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
	strcpy(q, "Input device file: ");
	strcpy(mode, "input");
    }
    else
    {
	ptr = cfg.onum;
	sprintf(tmp, "Output device config, current: %s", ptr);
	gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
	strcpy(q, "Output device file: ");
	strcpy(mode, "output");
    }

    get_user_input( q, tmp, ptr );

    if (!strcmp(tmp, ""))
    {
	sprintf(tmp, "No new %s device, using previous: %s", mode, ptr);
    	gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
	return ;
    }

    strcpy(ptr, tmp);

    sprintf(tmp, "New %s device: %s", mode, ptr);
    gtk_label_set_text( GTK_LABEL( glob_l ), tmp );
}

void btn_cfg(GtkWidget* _w, gpointer _l)
{
    gtk_label_set_text( GTK_LABEL( _l ), "Configuration" );

    GtkWidget *w, *t, *s, *l, *c, *cl, *bi, *bo, *bd;
    GtkWidget *bc, *bq, *bx, *bdb, *bwi, *bsa, *bpi;
    GtkWidget *biv, *bov, *bhz, *bsy, *bti, *bip, *bpo;

    w = gtk_window_new( GTK_WINDOW_TOPLEVEL );

    gtk_window_set_title( GTK_WINDOW( w ), "GCVoice: configuration");
    gtk_window_set_default_size( GTK_WINDOW( w ), 10, 10);

    t = gtk_table_new(7, 3, TRUE);
    gtk_table_set_row_spacings( GTK_TABLE( t ), 2 );
    gtk_table_set_col_spacings( GTK_TABLE( t ), 2 );

    gtk_container_add(GTK_CONTAINER( w ), t);

    l = gtk_button_new_with_label( "Load" );
    gtk_table_attach_defaults( GTK_TABLE( t ), l, 0, 1, 0, 1);

    s = gtk_button_new_with_label( "Save" );
    gtk_table_attach_defaults( GTK_TABLE( t ), s, 1, 2, 0, 1);

    c = gtk_button_new_with_label( "Clear" );
    gtk_table_attach_defaults( GTK_TABLE( t ), c, 2, 3, 0, 1);

    bi = gtk_button_new_with_label( "Input dev" );
    gtk_table_attach_defaults( GTK_TABLE( t ), bi, 0, 1, 1, 2);

    bo = gtk_button_new_with_label( "Output dev" );
    gtk_table_attach_defaults( GTK_TABLE( t ), bo, 1, 2, 1, 2);

    bd = gtk_button_new_with_label( "Display" );
    gtk_table_attach_defaults( GTK_TABLE( t ), bd, 2, 3, 1, 2);

    bc = gtk_button_new_with_label( "Cache" );
    gtk_table_attach_defaults( GTK_TABLE( t ), bc, 0, 1, 2, 3);

    bq = gtk_button_new_with_label( "Quality" );
    gtk_table_attach_defaults( GTK_TABLE( t ), bq, 1, 2, 2, 3);

    bx = gtk_button_new_with_label( "Complexity" );
    gtk_table_attach_defaults( GTK_TABLE( t ), bx, 2, 3, 2, 3);

    bdb = gtk_button_new_with_label( "Debug" );
    gtk_table_attach_defaults( GTK_TABLE( t ), bdb, 0, 1, 3, 4);

    bwi = gtk_button_new_with_label( "Format" );
    gtk_table_attach_defaults( GTK_TABLE( t ), bwi, 1, 2, 3, 4);

    bsa = gtk_button_new_with_label( "Dump" );
    gtk_table_attach_defaults( GTK_TABLE( t ), bsa, 2, 3, 3, 4);

    biv = gtk_button_new_with_label( "Input vol" );
    gtk_table_attach_defaults( GTK_TABLE( t ), biv, 0, 1, 4, 5);

    bov = gtk_button_new_with_label( "Output vol" );
    gtk_table_attach_defaults( GTK_TABLE( t ), bov, 1, 2, 4, 5);

    bhz = gtk_button_new_with_label( "Sampling" );
    gtk_table_attach_defaults( GTK_TABLE( t ), bhz, 2, 3, 4, 5);

    bsy = gtk_button_new_with_label( "Synchro" );
    gtk_table_attach_defaults( GTK_TABLE( t ), bsy, 0, 1, 5, 6);

    bti = gtk_button_new_with_label( "Timeout" );
    gtk_table_attach_defaults( GTK_TABLE( t ), bti, 1, 2, 5, 6);

    bpi = gtk_button_new_with_label( "PID logs" );
    gtk_table_attach_defaults( GTK_TABLE( t ), bpi, 2, 3, 5, 6);

    bip = gtk_button_new_with_label( "IP" );
    gtk_table_attach_defaults( GTK_TABLE( t ), bip, 0, 1, 6, 7);

    bpo = gtk_button_new_with_label( "Port" );
    gtk_table_attach_defaults( GTK_TABLE( t ), bpo, 1, 2, 6, 7);

    cl = gtk_button_new_with_label( "Close" );
    gtk_table_attach_defaults( GTK_TABLE( t ), cl, 2, 3, 6, 7);

    gtk_widget_show_all( w );


    g_signal_connect( s, "clicked", G_CALLBACK(btn_cfg_save), _l);
    g_signal_connect( l, "clicked", G_CALLBACK(btn_cfg_load), _l);
    g_signal_connect( c, "clicked", G_CALLBACK(btn_cfg_clear), _l);
    g_signal_connect( bi, "clicked", G_CALLBACK(btn_cfg_iofile), (gpointer)0);
    g_signal_connect( bo, "clicked", G_CALLBACK(btn_cfg_iofile), (gpointer)1);
    g_signal_connect( bd, "clicked", G_CALLBACK(btn_cfg_display), _l);
    g_signal_connect( bc, "clicked", G_CALLBACK(btn_cfg_intopt), (gpointer)0);
    g_signal_connect( bq, "clicked", G_CALLBACK(btn_cfg_intopt), (gpointer)1);
    g_signal_connect( bx, "clicked", G_CALLBACK(btn_cfg_intopt), (gpointer)2);
    g_signal_connect( bdb, "clicked", G_CALLBACK(btn_cfg_toggleopt), (gpointer)0);
    g_signal_connect( bwi, "clicked", G_CALLBACK(btn_cfg_toggleopt), (gpointer)1);
    g_signal_connect( bsa, "clicked", G_CALLBACK(btn_cfg_toggleopt), (gpointer)2);
    g_signal_connect( biv, "clicked", G_CALLBACK(btn_cfg_intopt), (gpointer)3);
    g_signal_connect( bov, "clicked", G_CALLBACK(btn_cfg_intopt), (gpointer)4);
    g_signal_connect( bhz, "clicked", G_CALLBACK(btn_cfg_intopt), (gpointer)5);
    g_signal_connect( bsy, "clicked", G_CALLBACK(btn_cfg_intopt), (gpointer)6);
    g_signal_connect( bti, "clicked", G_CALLBACK(btn_cfg_intopt), (gpointer)7);
    g_signal_connect( bpi, "clicked", G_CALLBACK(btn_cfg_toggleopt), (gpointer)3);
    g_signal_connect( bip, "clicked", G_CALLBACK(btn_cfg_ip), _l);
    g_signal_connect( bpo, "clicked", G_CALLBACK(btn_cfg_intopt), (gpointer)8);
    g_signal_connect_swapped( cl, "clicked", G_CALLBACK( gtk_widget_destroy ), w);
}

void gcvoice()
{
    GtkWidget *w, *l, *b[5], *t;

    strcpy(glob_last_cfg, "gcvoice.cfg" );
    pidlog = 0;
    port = 9000;
    strcpy(ip, "127.0.0.1");

    w = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title( GTK_WINDOW( w ), "GCVoice");
    gtk_window_set_default_size( GTK_WINDOW( w ), 10, 10);
    gtk_window_set_position( GTK_WINDOW( w ), GTK_WIN_POS_CENTER);

    t = gtk_table_new(2, 6, TRUE);
    gtk_table_set_row_spacings( GTK_TABLE( t ), 2 );
    gtk_table_set_col_spacings( GTK_TABLE( t ), 2 );

    b[0] = gtk_button_new_with_label( "Server" );
    gtk_table_attach_defaults( GTK_TABLE( t ), b[0], 1, 2, 0, 1);

    b[1] = gtk_button_new_with_label( "Client" );
    gtk_table_attach_defaults( GTK_TABLE( t ), b[1], 2, 3, 0, 1);

    b[2] = gtk_button_new_with_label( "Configure" );
    gtk_table_attach_defaults( GTK_TABLE( t ), b[2], 3, 4, 0, 1);

    b[3] = gtk_button_new_with_label( "Stop" );
    gtk_table_attach_defaults( GTK_TABLE( t ), b[3], 4, 5, 0, 1);

    b[4] = gtk_button_new_with_label( "Quit" );
    gtk_table_attach_defaults( GTK_TABLE( t ), b[4], 5, 6, 0, 1);

    b[5] = gtk_button_new_with_label( "Load" );
    gtk_table_attach_defaults( GTK_TABLE( t ), b[5], 0, 1, 0, 1);

    l = gtk_label_new("GCVoice");
    //gtk_label_set_justify( GTK_LABEL( l ), GTK_JUSTIFY_LEFT );
    gtk_misc_set_alignment( GTK_MISC( l ), 0, 0);
    gtk_table_attach_defaults( GTK_TABLE( t ), l, 0, 6, 1, 2);

    gtk_container_add(GTK_CONTAINER( w ), t);

    gtk_widget_show_all( w );

    g_signal_connect_swapped( G_OBJECT( w ), "destroy", G_CALLBACK( gtk_main_quit ), NULL);

    g_signal_connect( b[5], "clicked", G_CALLBACK( btn_cfg_load ), l);
    g_signal_connect( b[0], "clicked", G_CALLBACK( btn_server ), l);
    g_signal_connect( b[1], "clicked", G_CALLBACK( btn_client ), l);
    g_signal_connect( b[2], "clicked", G_CALLBACK( btn_cfg ), l);
    g_signal_connect( b[3], "clicked", G_CALLBACK( btn_stop ), l);
    g_signal_connect( b[4], "clicked", G_CALLBACK( gtk_main_quit ), NULL );

    glob_l = l;

    gtk_main();
}

int main(int lb, char** par)
{
    gtk_init(&lb, &par);
    gcvoice();
    return 0;
}

