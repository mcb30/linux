#ifndef _SOCKTSC_H
#define _SOCKTSC_H

//
struct sock_tsc {
	uint64_t ref;
	uint32_t tcp_sendmsg_lock_sock;
	uint32_t tcp_write_xmit;
	uint32_t ip_queue_xmit;
	uint32_t dev_queue_xmit;
	uint32_t tcp_sendmsg_out;
	uint32_t tcp_sendmsg_release_sock;
};

//
static inline uint64_t sk_tsc_now ( void ) {
	uint32_t eax;
	uint32_t edx;
	uint32_t ecx;

	__asm__ __volatile__ ( "rdtscp"
			       : "=a" ( eax ), "=d" ( edx ), "=c" ( ecx ) );
	return ( ( ( ( uint64_t ) edx ) << 32 ) | eax );
}

//
static inline uint32_t sk_tsc_delta ( struct sock_tsc *tsc ) {
	uint64_t now = sk_tsc_now();
	uint32_t delta = ( now - tsc->ref );
	tsc->ref = now;
	return delta;
}

//
static inline struct sock_tsc * sk_tsc ( struct sock *sk ) {
	return ( ( struct sock_tsc * ) &(sk)->rh_reserved1 );
}

//
#define SK_TSC( sk, field ) do {					\
		struct sock_tsc *tsc = sk_tsc ( (sk) );			\
		tsc->field = sk_tsc_delta ( tsc );			\
	} while ( 0 )

#endif /* _SOCKTSC_H */

