#ifndef SIM_H
#define SIM_H

#include <stdarg.h>
#include <linux/types.h>
#include <linux/sched.h>

#include "sim-types.h"

struct SimTask {
	struct list_head head;
	struct task_struct kernel_task;
	void *private;
};

/* API called from within linux kernel. Forwards to SimImported. */
int sim_vprintf(const char *str, va_list args);
void *sim_malloc(unsigned long size);
void sim_free(void *buffer);
void *sim_memcpy(void *dst, const void *src, unsigned long size);
void *sim_memset(void *dst, char value, unsigned long size);
unsigned long sim_random(void);
void *sim_event_schedule_ns(__u64 ns, void (*fn) (void *context),
			    void *context);
void sim_event_cancel(void *event);
__u64 sim_current_ns(void);

struct SimTask *sim_task_start(void (*callback) (void *), void *context);
void sim_task_wait(void);
void sim_task_yield(void);
struct SimTask *sim_task_current(void);
/* returns 1 if task was woken up, 0 if it was already running. */
int sim_task_wakeup(struct SimTask *task);
struct SimTask *sim_task_create(void *private, unsigned long pid);
void sim_task_destroy(struct SimTask *task);

void sim_dev_xmit(struct SimDevice *dev, unsigned char *data, int len);
struct SimDevicePacket sim_dev_create_packet(struct SimDevice *dev, int size);
void sim_dev_rx(struct SimDevice *device, struct SimDevicePacket packet);

void sim_signal_raised(struct SimTask *task, int sig);

void sim_poll_event(int flag, void *context);
void sim_softirq_wakeup(void);
void sim_update_jiffies(void);
void *sim_dev_get_private(struct SimDevice *);
struct SimDevice *sim_dev_create(char *ifname,
				 void *priv, enum SimDevFlags flags);
void sim_dev_set_address(struct SimDevice *dev, unsigned char buffer[6]);
void sim_proc_net_initialize(void);

#endif /* SIM_H */
