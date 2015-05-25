################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/connect_nonb.c \
../src/connect_timeo.c \
../src/daemon_inetd.c \
../src/daemon_init.c \
../src/dg_cli.c \
../src/dg_echo.c \
../src/error.c \
../src/family_to_level.c \
../src/get_ifi_info.c \
../src/gf_time.c \
../src/host_serv.c \
../src/hstrerror.c \
../src/if_indextoname.c \
../src/if_nameindex.c \
../src/if_nametoindex.c \
../src/in6addr_any.c \
../src/mcast_get_if.c \
../src/mcast_get_loop.c \
../src/mcast_get_ttl.c \
../src/mcast_join.c \
../src/mcast_leave.c \
../src/mcast_set_if.c \
../src/mcast_set_loop.c \
../src/mcast_set_ttl.c \
../src/my_addrs.c \
../src/pselect.c \
../src/read_fd.c \
../src/readable_timeo.c \
../src/readline.c \
../src/readn.c \
../src/rtt.c \
../src/signal.c \
../src/signal_intr.c \
../src/snprintf.c \
../src/sock_bind_wild.c \
../src/sock_cmp_addr.c \
../src/sock_cmp_port.c \
../src/sock_get_port.c \
../src/sock_ntop.c \
../src/sock_ntop_host.c \
../src/sock_set_addr.c \
../src/sock_set_port.c \
../src/sock_set_wild.c \
../src/sockatmark.c \
../src/sockfd_to_family.c \
../src/str_cli.c \
../src/str_echo.c \
../src/tcp_connect.c \
../src/tcp_listen.c \
../src/tv_sub.c \
../src/udp_client.c \
../src/udp_connect.c \
../src/udp_server.c \
../src/wraplib.c \
../src/wrappthread.c \
../src/wrapsock.c \
../src/wrapstdio.c \
../src/wrapunix.c \
../src/writable_timeo.c \
../src/write_fd.c \
../src/writen.c 

OBJS += \
./src/connect_nonb.o \
./src/connect_timeo.o \
./src/daemon_inetd.o \
./src/daemon_init.o \
./src/dg_cli.o \
./src/dg_echo.o \
./src/error.o \
./src/family_to_level.o \
./src/get_ifi_info.o \
./src/gf_time.o \
./src/host_serv.o \
./src/hstrerror.o \
./src/if_indextoname.o \
./src/if_nameindex.o \
./src/if_nametoindex.o \
./src/in6addr_any.o \
./src/mcast_get_if.o \
./src/mcast_get_loop.o \
./src/mcast_get_ttl.o \
./src/mcast_join.o \
./src/mcast_leave.o \
./src/mcast_set_if.o \
./src/mcast_set_loop.o \
./src/mcast_set_ttl.o \
./src/my_addrs.o \
./src/pselect.o \
./src/read_fd.o \
./src/readable_timeo.o \
./src/readline.o \
./src/readn.o \
./src/rtt.o \
./src/signal.o \
./src/signal_intr.o \
./src/snprintf.o \
./src/sock_bind_wild.o \
./src/sock_cmp_addr.o \
./src/sock_cmp_port.o \
./src/sock_get_port.o \
./src/sock_ntop.o \
./src/sock_ntop_host.o \
./src/sock_set_addr.o \
./src/sock_set_port.o \
./src/sock_set_wild.o \
./src/sockatmark.o \
./src/sockfd_to_family.o \
./src/str_cli.o \
./src/str_echo.o \
./src/tcp_connect.o \
./src/tcp_listen.o \
./src/tv_sub.o \
./src/udp_client.o \
./src/udp_connect.o \
./src/udp_server.o \
./src/wraplib.o \
./src/wrappthread.o \
./src/wrapsock.o \
./src/wrapstdio.o \
./src/wrapunix.o \
./src/writable_timeo.o \
./src/write_fd.o \
./src/writen.o 

C_DEPS += \
./src/connect_nonb.d \
./src/connect_timeo.d \
./src/daemon_inetd.d \
./src/daemon_init.d \
./src/dg_cli.d \
./src/dg_echo.d \
./src/error.d \
./src/family_to_level.d \
./src/get_ifi_info.d \
./src/gf_time.d \
./src/host_serv.d \
./src/hstrerror.d \
./src/if_indextoname.d \
./src/if_nameindex.d \
./src/if_nametoindex.d \
./src/in6addr_any.d \
./src/mcast_get_if.d \
./src/mcast_get_loop.d \
./src/mcast_get_ttl.d \
./src/mcast_join.d \
./src/mcast_leave.d \
./src/mcast_set_if.d \
./src/mcast_set_loop.d \
./src/mcast_set_ttl.d \
./src/my_addrs.d \
./src/pselect.d \
./src/read_fd.d \
./src/readable_timeo.d \
./src/readline.d \
./src/readn.d \
./src/rtt.d \
./src/signal.d \
./src/signal_intr.d \
./src/snprintf.d \
./src/sock_bind_wild.d \
./src/sock_cmp_addr.d \
./src/sock_cmp_port.d \
./src/sock_get_port.d \
./src/sock_ntop.d \
./src/sock_ntop_host.d \
./src/sock_set_addr.d \
./src/sock_set_port.d \
./src/sock_set_wild.d \
./src/sockatmark.d \
./src/sockfd_to_family.d \
./src/str_cli.d \
./src/str_echo.d \
./src/tcp_connect.d \
./src/tcp_listen.d \
./src/tv_sub.d \
./src/udp_client.d \
./src/udp_connect.d \
./src/udp_server.d \
./src/wraplib.d \
./src/wrappthread.d \
./src/wrapsock.d \
./src/wrapstdio.d \
./src/wrapunix.d \
./src/writable_timeo.d \
./src/write_fd.d \
./src/writen.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


