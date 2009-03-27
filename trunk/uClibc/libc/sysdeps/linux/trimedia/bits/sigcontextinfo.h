/*
 * FIXME: It is not suitable for trimedia. And need to reimplement.
 */

#define SIGCONTEXT int _code, struct sigcontext *
#define SIGCONTEXT_EXTRA_ARGS _code,
#define GET_PC(ctx)	((void *) (ctx)->sc_pc)
#define GET_FRAME(ctx)	((void *) __builtin_frame_address (1))
#define GET_STACK(ctx)	((void *) (ctx)->sc_usp)
#define CALL_SIGHANDLER(handler, signo, ctx) \
  (handler)((signo), SIGCONTEXT_EXTRA_ARGS (ctx))
