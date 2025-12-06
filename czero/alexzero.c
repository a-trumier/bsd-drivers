/*
 * A character zero driver.
 */

#include <sys/types.h>
#include <sys/systm.h>
#include <sys/errno.h>
#include <sys/param.h>
#include <sys/module.h>
#include <sys/kernel.h>

#define BUFSIZE 256

MALLOC_DECLARE(M_BUF);
MALLOC_DEFINE(M_BUF, "buffer", "buffer for zeros");

static struct cdevsw alexzero_cdevsw = {
	.d_version = D_VERSION,
	.d_open = alexzero_open,
	.d_close = alexzero_close,
	.d_read = alexzero_read,
	.d_write = alexzero_write,
	.d_name = "alexzero",
};

static char *buf;

static int
alexzero_loader(struct module *m __unused, int what, void *arg __unused)
{
	int error = 0;

	switch (what) {
	case MOD_LOAD:                /* kldload */
		error = make_dev_p(MAKEDEV_CHECKNAME | MAKEDEV_WAITOK,
		    &alexzero_dev,
		    &alexzero_cdevsw,
		    0,
		    UID_ROOT,
		    GID_WHEEL,
		    0600,
		    "alexzero");
		if (error != 0)
			break;
		break;
	case MOD_UNLOAD:
		destroy_dev(alexzero_dev);
        free()
		printf("alexzero device unloaded.\n");
		break;
	default:
		error = EOPNOTSUPP;
		break;
	}
	return (error);
}
static int
alexzero_open(struct cdev *dev __unused, int oflags __unused, int devtype __unused,
    struct thread *td __unused)
{
	int error = 0;

	uprintf("Opened device \"alexzero\" successfully.\n");
	return (error);
}

static int
alexzero_close(struct cdev *dev __unused, int fflag __unused, int devtype __unused,
    struct thread *td __unused)
{

	uprintf("Closing device \"alexzero\".\n");
	return (0);
}

static int
alexzero_read(struct cdev *dev __unused, struct uio *uio, int ioflag __unused)
{
	size_t amt;
	int error;

    /* First, allocate a buffer of the correct size to pass into userland */
	amt = uio->uio_resid;
    buf = malloc((amt) * sizeof(char), M_BUF, M_WAITOK | M_ZERO);
    memset(buf, 0, amt);


	if ((error = uiomove(buf, amt, uio)) != 0)
		uprintf("uiomove failed!\n");

    free(buf);

	return (error);
}

static int
alexzero_write(struct cdev *dev __unused, struct uio *uio, int ioflag __unused)
{
    /* ERROR: Cannot write to zero */
    return EINVAL;
}

DEV_MODULE(alexzero, alexzero_loader, NULL);
