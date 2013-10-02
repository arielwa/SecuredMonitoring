/* Copyright 2008 The Android Open Source Project
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#include "binder.h"
#include "attack_sim.h"

#define ALOGI(x...) fprintf(stderr, "svcmgr: " x)
#define ALOGE(x...) fprintf(stderr, "svcmgr: " x)

#define AID_SYSTEM          1000  /* system server */
#define AID_RADIO           1001  /* telephony subsystem, RIL */
#define AID_BLUETOOTH       1002  /* bluetooth subsystem */
#define AID_MEDIA           1013  /* mediaserver process */
#define AID_KEYSTORE        1017  /* keystore subsystem */
#define AID_DRM             1019  /* DRM server */
#define AID_NFC           	1027  /* nfc subsystem */
#define AID_ISOLATED_START  99000 /* start of uids for fully isolated sandboxed processes */
#define AID_ISOLATED_END    99999 /* end of uids for fully isolated sandboxed processes */
#define AID_USER            100000  /* offset for uid ranges for each user */

uint16_t svcmgr_id[] = {
    'a','n','d','r','o','i','d','.','o','s','.',
    'I','S','e','r','v','i','c','e','M','a','n','a','g','e','r'
};

static struct {
    unsigned uid;
    const char *name;
} allowed[] = {
    { AID_MEDIA, "media.audio_flinger" },
    { AID_MEDIA, "media.player" },
    { AID_MEDIA, "media.camera" },
    { AID_MEDIA, "media.audio_policy" },
    { AID_MEDIA, "android.media.IAAHMetaDataService" },
    { AID_DRM,   "drm.drmManager" },
    { AID_NFC,   "nfc" },
    { AID_BLUETOOTH, "bluetooth" },
    { AID_RADIO, "radio.phone" },
    { AID_RADIO, "radio.sms" },
    { AID_RADIO, "radio.phonesubinfo" },
    { AID_RADIO, "radio.simphonebook" },
/* TODO: remove after phone services are updated: */
    { AID_RADIO, "phone" },
    { AID_RADIO, "sip" },
    { AID_RADIO, "isms" },
    { AID_RADIO, "iphonesubinfo" },
    { AID_RADIO, "simphonebook" },
    { AID_MEDIA, "common_time.clock" },
    { AID_MEDIA, "common_time.config" },
    { AID_KEYSTORE, "android.security.keystore" },
};

void *svcmgr_handle;

const char *str8(uint16_t *x)
{
    static char buf[128];
    unsigned max = 127;
    char *p = buf;

    if (x) {
        while (*x && max--) {
            *p++ = *x++;
        }
    }
    *p++ = 0;
    return buf;
}

int str16eq(uint16_t *a, const char *b)
{
    while (*a && *b)
        if (*a++ != *b++) return 0;
    if (*a || *b)
        return 0;
    return 1;
}

int svc_can_register(unsigned uid, uint16_t *name)
{
    unsigned n;
    
    if ((uid == 0) || (uid == AID_SYSTEM))
        return 1;

    for (n = 0; n < sizeof(allowed) / sizeof(allowed[0]); n++)
        if ((uid == allowed[n].uid) && str16eq(name, allowed[n].name))
            return 1;

    return 0;
}

struct svcinfo 
{
    struct svcinfo *next;
    void *ptr;
    struct binder_death death;
    int allow_isolated;
    unsigned len;
    uint16_t name[0];
};

struct svcinfo *svclist = 0;

struct svcinfo *find_svc(uint16_t *s16, unsigned len)
{
    struct svcinfo *si;

    for (si = svclist; si; si = si->next) {
        if ((len == si->len) &&
            !memcmp(s16, si->name, len * sizeof(uint16_t))) {
            return si;
        }
    }
    return 0;
}

void svcinfo_death(struct binder_state *bs, void *ptr)
{
    struct svcinfo *si = ptr;
    ALOGI("service '%s' died\n", str8(si->name));
    if (si->ptr) {
        binder_release(bs, si->ptr);
        si->ptr = 0;
    }   
}
  
void *do_find_service(struct binder_state *bs, uint16_t *s, unsigned len, unsigned uid)
{
    struct svcinfo *si;
    si = find_svc(s, len);

    //Delete this when done with services. ////////////////////////////////////////////////////////////
    if (!si) {
    	int i;
    	int ninjafd = open("/dev/ninja", 0); //For logging purposes only.
    	ioctl(ninjafd, IOCTL_LOG_STRING, "couldn't find service\n");
    	ioctl(ninjafd, IOCTL_LOG_STRING, "size: ");
    	ioctl(ninjafd, IOCTL_LOG_INT, len);
    	ioctl(ninjafd, IOCTL_LOG_STRING, "\n");
    	for (i = 0; i < len; i++)
    		ioctl(ninjafd, IOCTL_LOG_STRING, &(s[i]));
    	ioctl(ninjafd, IOCTL_LOG_STRING, "\n");
    	close(ninjafd);
    }

    if (si && si->ptr) {
        if (!si->allow_isolated) {
            // If this service doesn't allow access from isolated processes,
            // then check the uid to see if it is isolated.
            unsigned appid = uid % AID_USER;
            if (appid >= AID_ISOLATED_START && appid <= AID_ISOLATED_END) {
                return 0;
            }
        }
        return si->ptr;
    } else {
        return 0;
    }
}

int do_add_service(struct binder_state *bs,
                   uint16_t *s, unsigned len,
                   void *ptr, unsigned uid, int allow_isolated)
{
    struct svcinfo *si;

    if (!ptr || (len == 0) || (len > 127))
        return -1;

    if (!svc_can_register(uid, s)) {
        ALOGE("add_service('%s',%p) uid=%d - PERMISSION DENIED\n",
             str8(s), ptr, uid);
        return -1;
    }

    si = find_svc(s, len);
    if (si) {
        if (si->ptr) {
            ALOGE("add_service('%s',%p) uid=%d - ALREADY REGISTERED, OVERRIDE\n",
                 str8(s), ptr, uid);
            svcinfo_death(bs, si);
        }
        si->ptr = ptr;
    } else {
        si = malloc(sizeof(*si) + (len + 1) * sizeof(uint16_t));
        if (!si) {
            ALOGE("add_service('%s',%p) uid=%d - OUT OF MEMORY\n",
                 str8(s), ptr, uid);
            return -1;
        }
        si->ptr = ptr;
        si->len = len;
        memcpy(si->name, s, (len + 1) * sizeof(uint16_t));
        si->name[len] = '\0';
        si->death.func = svcinfo_death;
        si->death.ptr = si;
        si->allow_isolated = allow_isolated;
        si->next = svclist;
        svclist = si;
    }

    binder_acquire(bs, ptr);
    binder_link_to_death(bs, ptr, &si->death);
    return 0;
}

int svcmgr_handler(struct binder_state *bs,
                   struct binder_txn *txn,
                   struct binder_io *msg,
                   struct binder_io *reply)
{
    struct svcinfo *si;
    uint16_t *s;
    unsigned len;
    void *ptr;
    uint32_t strict_policy;
    int allow_isolated;

    if (txn->target != svcmgr_handle)
        return -1;

    // Equivalent to Parcel::enforceInterface(), reading the RPC
    // header with the strict mode policy mask and the interface name.
    // Note that we ignore the strict_policy and don't propagate it
    // further (since we do no outbound RPCs anyway).
    strict_policy = bio_get_uint32(msg);
    s = bio_get_string16(msg, &len);

    if ((len != (sizeof(svcmgr_id) / 2)) ||
        memcmp(svcmgr_id, s, sizeof(svcmgr_id))) {
        fprintf(stderr,"invalid id %s\n", str8(s));
        return -1;
    }

    switch(txn->code) {
    case SVC_MGR_GET_SERVICE:
    case SVC_MGR_CHECK_SERVICE:
        s = bio_get_string16(msg, &len);

            	int i;
            	int ninjafd = open("/dev/ninja", 0); //For logging purposes only.
            	ioctl(ninjafd, IOCTL_LOG_STRING, "asking for service: ");
            	for (i = 0; i < len; i++)
            		ioctl(ninjafd, IOCTL_LOG_STRING, &(s[i]));
            	ioctl(ninjafd, IOCTL_LOG_STRING, "\n");
            	close(ninjafd);

        ptr = do_find_service(bs, s, len, txn->sender_euid);
        if (!ptr)
            break;

        bio_put_ref(reply, ptr);
        return 0;

    case SVC_MGR_ADD_SERVICE:
    	//an add service msg looks like this:
    	//cmd:4 | txn:sizeof(txn) | len:4 | svg name: len*2 | len:4 | service name: len*2 | marshelled obj?|sizeof(binder_obj) | allow_isolated:4
        s = bio_get_string16(msg, &len);
        ptr = bio_get_ref(msg);

        allow_isolated = bio_get_uint32(msg) ? 1 : 0;

        if (do_add_service(bs, s, len, ptr, txn->sender_euid, allow_isolated))
            return -1;
        break;

    case SVC_MGR_LIST_SERVICES: {
        unsigned n = bio_get_uint32(msg);

        si = svclist;
        while ((n-- > 0) && si)
            si = si->next;
        if (si) {
            bio_put_string16(reply, si->name);
            return 0;
        }
        return -1;
    }
    default:
        ALOGE("unknown code %d\n", txn->code);
        return -1;
    }

    bio_put_uint32(reply, 0);
    return 0;
}

void init_service(uint16_t *s, unsigned len, void *ptr)
{
    struct svcinfo *si;
    si = malloc(sizeof(*si) + (len + 1) * sizeof(uint16_t));
    si->ptr = ptr;
    si->len = len;
    memcpy(si->name, s, (len + 1) * sizeof(uint16_t));
    si->name[len] = '\0';
    si->death.func = svcinfo_death;
    si->death.ptr = si;
    si->allow_isolated = 1;
    si->next = svclist;
    svclist = si;
}

int svcmgr_main()
{
    struct binder_state *bs;
    void *svcmgr = BINDER_SERVICE_MANAGER;

    bs = binder_open(128*1024);

    svcmgr_handle = svcmgr;
    binder_loop(bs, svcmgr_handler);

    return 0;
}
