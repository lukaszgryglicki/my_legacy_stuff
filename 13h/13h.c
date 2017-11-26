     /*
      	VM8086 driver for mode 13H
      */
     #define MIN(a,b) (((a) < (b)) ? (a) : (b))

     #include <sys/types.h>
     #include <sys/module.h>
     #include <sys/systm.h> /* uprintf */
     #include <sys/errno.h>
     #include <sys/param.h>  /* defines used in kernel.h */
     #include <sys/kernel.h> /* types used in module initialization */
     #include <sys/conf.h>   /* cdevsw struct */
     #include <sys/uio.h>    /* uio struct */
     #include <sys/malloc.h>

     #define BUFFERSIZE 256

     /* Function prototypes */
     d_open_t      m13h_open;
     d_close_t     m13h_close;
     d_read_t      m13h_read;
     d_write_t     m13h_write;

extern int goto_vm86();

     /* Character device entry points */
     static struct cdevsw echo_cdevsw = {
       m13h_open,
       m13h_close,
       m13h_read,
       m13h_write,
       noioctl,
       nopoll,
       nommap,
       nostrategy,
       "m13h",
       33,                   /* reserved for lkms - /usr/src/sys/conf/majors */
       nodump,
       nopsize,
       D_TTY,
       -1
     };

     typedef struct s_m13h {
       char msg[BUFFERSIZE];
       int len;
     } t_m13h;

     /* vars */
     static dev_t sdev;
     static int len;
     static int count;
     static t_m13h *m13hmsg;

     MALLOC_DECLARE(M_M13HBUF);
     MALLOC_DEFINE(M_M13HBUF, "m13hbuffer", "buffer for 13H module");

     /*
      * This function acts is called by the kld[un]load(2) system calls to
      * determine what actions to take when a module is loaded or unloaded.
      */

     static int
     m13h_loader(struct module *m, int what, void *arg)
     {
       int err = 0;

       switch (what) {
       case MOD_LOAD:                /* kldload */
         sdev = make_dev(&echo_cdevsw,
                     0,
                     UID_ROOT,
                     GID_WHEEL,
                     0600,
                     "echo");
         /* kmalloc memory for use by this driver */
         /*    malloc(256,M_ECHOBUF,M_WAITOK); */
         MALLOC(m13hmsg, t_m13h *, sizeof(t_m13h), M_M13HBUF, M_WAITOK);
         printf("Mode 13H device loaded.\n");
	 printf("Now going to REAL_VM86_MODE <expect_panic>\n");
	 goto_vm86();
	 printf("We're here so we survived ASSEMBLER-control kernel\n");
         break;
       case MOD_UNLOAD:
         destroy_dev(sdev);
         FREE(m13hmsg,M_M13HBUF);
         printf("Mode 13H device unloaded.\n");
         break;
       default:
         err = EINVAL;
         break;
       }
       return(err);
     }

     int
     m13h_open(dev_t dev, int oflags, int devtype, struct proc *p)
     {
       int err = 0;

       uprintf("Opened device \"m13h\" successfully.\n");
       return(err);
     }

     int
     m13h_close(dev_t dev, int fflag, int devtype, struct proc *p)
     {
       uprintf("Closing device \"m13h.\"\n");
       return(0);
     }

     /*
      * The read function just takes the buf that was saved via
      * echo_write() and returns it to userland for accessing.
      * uio(9)
      */

     int
     m13h_read(dev_t dev, struct uio *uio, int ioflag)
     {
       int err = 0;
       int amt;

       /* How big is this read operation?  Either as big as the user wants,
          or as big as the remaining data */
       amt = MIN(uio->uio_resid, (m13hmsg->len - uio->uio_offset > 0) ? m13hmsg->len - uio->uio_offset : 0);
       if ((err = uiomove(m13hmsg->msg + uio->uio_offset,amt,uio)) != 0) {
         uprintf("uiomove failed!\n");
       }

       return err;
     }

     /*
      * echo_write takes in a character string and saves it
      * to buf for later accessing.
      */

     int
     m13h_write(dev_t dev, struct uio *uio, int ioflag)
     {
       int err = 0;

       /* Copy the string in from user memory to kernel memory */
       err = copyin(uio->uio_iov->iov_base, m13hmsg->msg, MIN(uio->uio_iov->iov_len,BUFFERSIZE));

       /* Now we need to null terminate */
       *(m13hmsg->msg + MIN(uio->uio_iov->iov_len,BUFFERSIZE)) = 0;
       /* Record the length */
       m13hmsg->len = MIN(uio->uio_iov->iov_len,BUFFERSIZE);

       if (err != 0) {
         uprintf("Write failed: bad address!\n");
       }

       count++;
       return(err);
     }

     DEV_MODULE(m13h,m13h_loader,NULL);

