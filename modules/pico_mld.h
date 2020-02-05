/*********************************************************************
 * PicoTCP-NG 
 * Copyright (c) 2020 Daniele Lacamera <root@danielinux.net>
 *
 * This file also includes code from:
 * PicoTCP
 * Copyright (c) 2012-2017 Altran Intelligent Systems
 * 
 * SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only
 *
 * PicoTCP-NG is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) version 3.
 *
 * PicoTCP-NG is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA
 *
 *
 *********************************************************************/
#ifndef INCLUDE_PICO_MLD
#define INCLUDE_PICO_MLD

#include <stdint.h>
#include "pico_ipv6.h"

#define PICO_MLDV1               1
#define PICO_MLDV2               2

#define PICO_MLD_QUERY  130
#define PICO_MLD_REPORT 131
#define PICO_MLD_DONE   132
#define PICO_MLD_REPORTV2 143

/*RFC 3810 $6.2 */
#define MLD_HOP_LIMIT 1

/* states */
#define MLD_STATE_NON_LISTENER              (0x0)
#define MLD_STATE_DELAYING_LISTENER         (0x1)
#define MLD_STATE_IDLE_LISTENER             (0x2)

#define PICO_MLD_STATE_CREATE    1
#define PICO_MLD_STATE_UPDATE    2
#define PICO_MLD_STATE_DELETE    3
/* group record types */
#define MLD_MODE_IS_INCLUDE                  (1)
#define MLD_MODE_IS_EXCLUDE                  (2)
#define MLD_CHANGE_TO_INCLUDE_MODE           (3)
#define MLD_CHANGE_TO_EXCLUDE_MODE           (4)
#define MLD_ALLOW_NEW_SOURCES            (5)
#define MLD_BLOCK_OLD_SOURCES            (6)
/* events */

#define MLD_EVENT_START_LISTENING           (0x1)
#define MLD_EVENT_STOP_LISTENING            (0x0)
#define MLD_EVENT_QUERY_RECV                (0x3)
#define MLD_EVENT_REPORT_RECV               (0x4)
#define MLD_EVENT_TIMER_EXPIRED             (0x5)
/*Not needed?*/
#define MLD_EVENT_DONE_RECV                 (0x1)

#define MLD_EVENT_DELETE_GROUP           (0x0)
#define MLD_EVENT_CREATE_GROUP           (0x1)
#define MLD_EVENT_UPDATE_GROUP           (0x2)
#define MLD_EVENT_QUERY_RECV             (0x3)
#define MLD_EVENT_REPORT_RECV            (0x4)
#define MLD_EVENT_TIMER_EXPIRED          (0x5)
/* (default) Variabels for times/counters */
/* ALL IN SECONDS */
#define MLD_ROBUSTNESS                      (2)
#define MLD_QUERY_INTERVAL                  (125)
#define MLD_QUERY_RESPONSE_INTERVAL         (10)
#define MLD_DEFAULT_MAX_RESPONSE_TIME       (100)
#define MLD_MULTICAST_LISTENER_INTERVAL     (MLD_ROBUSTNESS * MLD_QUERY_INTERVAL) + MLD_QUERY_RESPONSE_INTERVAL
#define MLD_OTHER_QUERIER_PRESENT_INTERVAL  (MLD_ROBUSTNESS * MLD_QUERY_INTERVAL) + (0.5 * MLD_QUERY_RESPONSE_INTERVAL)
#define MLD_STARTUP_QUERY_INTERVAL          (0.25 * MLD_QUERY_INTERVAL)
#define MLD_STARTUP_QUERY_COUNT             MLD_ROBUSTNESS
#define MLD_LAST_LISTENER_QUERY_INTERVAL    1
#define MLD_LISTENER_QUERY_COUNT            MLD_ROBUSTNESS
#define MLD_UNSOLICITED_REPORT_INTERVAL     10

/* custom timers types */
#define MLD_TIMER_GROUP_REPORT           (1)
#define MLD_TIMER_V1_QUERIER             (2)
#define MLD_TIMER_V2_QUERIER             (2)


/* Who has send the last report message */
#define MLD_HOST_LAST                    (0x1)
#define MLD_HOST_NOT_LAST                (0x0)


#define MLD_TIMER_STOPPED                (1)
#define MLD_MAX_SOURCES                  (89)
extern struct pico_protocol pico_proto_mld;

struct mld_multicast_address_record {
    uint8_t type;
    uint8_t aux_len;
    uint16_t nbr_src;
    struct pico_ip6 multicast;
    struct pico_ip6 src[1];
};

struct mld_parameters {
    uint8_t event;
    uint8_t state;
    uint8_t general_query;
    uint8_t filter_mode;
    uint8_t last_host;
    uint16_t max_resp_time;
    struct pico_ip6 mcast_link;
    struct pico_ip6 mcast_group;
    struct pico_tree *MCASTFilter;
    struct pico_frame *f;
};

struct mld_timer {
    uint8_t type;
    uint8_t stopped;
    pico_time start;
    pico_time delay;
    struct pico_ip6 mcast_link;
    struct pico_ip6 mcast_group;
    struct pico_frame *f;
    void (*mld_callback)(struct mld_timer *t);
    struct pico_stack *stack;
};

int mld_timer_cmp(void *ka, void *kb);
int mld_sources_cmp(void *ka, void *kb);
int mcast_parameters_cmp(void *ka, void *kb);
uint16_t pico_mld_checksum(struct pico_frame *f);
int pico_mld_process_in(struct pico_stack *S, struct pico_frame *f);
int pico_mld_state_change(struct pico_stack *S, struct pico_ip6 *mcast_link, struct pico_ip6 *mcast_group, uint8_t filter_mode, struct pico_tree *_MCASTFilter, uint8_t state);
#endif /* _INCLUDE_PICO_MLD */
