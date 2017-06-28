/**
 * Copyright (C) Mellanox Technologies Ltd. 2001-2017.  ALL RIGHTS RESERVED.
 *
 * See file LICENSE for terms.
 */

#ifndef UCP_TAG_MATCH_H_
#define UCP_TAG_MATCH_H_

#include <ucp/api/ucp_def.h>
#include <ucp/core/ucp_types.h>
#include <ucs/datastruct/queue_types.h>
#include <ucs/sys/compiler_def.h>


#define UCP_TAG_MASK_FULL     0xffffffffffffffffUL  /* All 1-s */


/**
 * Tag-match header
 */
typedef struct {
    ucp_tag_t                 tag;
} UCS_S_PACKED ucp_tag_hdr_t;


/**
 * Tag-matching context
 */
typedef struct ucp_tag_match {
    struct {
        ucs_queue_head_t      wildcard;   /* Expected wildcard requests */
        ucs_queue_head_t      *hash;      /* Hash table of expected non-wild tags */
        uint64_t              sn;
    } expected;
    struct {
        ucs_list_link_t       all;        /* Linked list of all tags */
        ucs_list_link_t       *hash;      /* Hash table of unexpected tags */
    } unexpected;

    /* Tag offload fields */
    ucs_queue_head_t          offload_ifaces; /* Interfaces which support tag offload */
    ucs_queue_head_t          sync_reqs;      /* Outgoing sync send requests */
    size_t                    thresh;         /* Messages are posted to the transport
                                                 starting from this threshold. Either user
                                                 buffer itself or preregistered internal
                                                 buffer is posted. */
    size_t                    zcopy_thresh;   /* Defines when user buffers should be posted
                                                 to the transport. */
    unsigned                  sw_req_count;   /* Number of requests which need to be matched
                                                 in software. If 0 - tags can be posted to the
                                                 transport */
} ucp_tag_match_t;


ucs_status_t ucp_tag_match_init(ucp_tag_match_t *tm);

void ucp_tag_match_cleanup(ucp_tag_match_t *tm);

void ucp_tag_exp_remove(ucp_tag_match_t *tm, ucp_request_t *req);

int ucp_tag_unexp_is_empty(ucp_tag_match_t *tm);

ucp_request_t*
ucp_tag_exp_search_all(ucp_tag_match_t *tm, ucs_queue_head_t *hash_queue,
                       ucp_tag_t recv_tag, size_t recv_len, unsigned recv_flags);

#endif
