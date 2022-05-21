#include "loopback.h"
#include "W5500/w5500.h"
#include "socket.h"
#include "common.h"
#include "wizchip_conf.h"
#include <stdio.h>

extern uint32_t GlobalCounter;

#if _WIZCHIP_SOCK_NUM_ == 8
uint8_t connection_status[_WIZCHIP_SOCK_NUM_] = {0,0,0,0,0,0,0,0};
#else
uint8_t connection_status[_WIZCHIP_SOCK_NUM_] = {0,0,0,0};
#endif

#if LOOPBACK_MODE == LOOPBACK_MAIN_NOBLCOK

int32_t loopback_tcps(uint8_t sn, uint8_t* buf, uint16_t port)
{
   int32_t ret;
   uint16_t size = 0, sentsize=0;
   switch(getSn_SR(sn))
   {
      case SOCK_ESTABLISHED :
         if(getSn_IR(sn) & Sn_IR_CON)
         {
            printf("%d:Connected\r\n",sn);
            setSn_IR(sn,Sn_IR_CON);
         }
         if((size = getSn_RX_RSR(sn)) > 0) // Don't need to check SOCKERR_BUSY because it doesn't not occur.
         {
            if(size > DATA_BUF_SIZE) size = DATA_BUF_SIZE;
            ret = recv(sn,buf,size);
            if(ret <= 0) return ret;      // check SOCKERR_BUSY & SOCKERR_XXX. For showing the occurrence of SOCKERR_BUSY.
            sentsize = 0;
            while(size != sentsize)
            {
               ret = send(sn,buf+sentsize,size-sentsize);
               if(ret < 0)
               {
                  close(sn);
                  return ret;
               }
               sentsize += ret; // Don't care SOCKERR_BUSY, because it is zero.
            }
         }
         break;
      case SOCK_CLOSE_WAIT :
         printf("%d:CloseWait\r\n",sn);
         if((ret=disconnect(sn)) != SOCK_OK) return ret;
         printf("%d:Closed\r\n",sn);
         break;
      case SOCK_INIT :
         printf("%d:Listen\r\n",sn);
         if( (ret = listen(sn)) != SOCK_OK) return ret;
         break;
      case SOCK_CLOSED:
         printf("%d:LBTStart\r\n",sn);
         if((ret=socket(sn,Sn_MR_TCP,port,0x00)) != sn)
            return ret;
         printf("%d:Opened\r\n",sn);
         break;
      default:
         break;
   }
   return 1;
}

int32_t loopback_udps(uint8_t sn, uint8_t* buf, uint16_t port)
{
   int32_t  ret;
   uint16_t size, sentsize;
   uint8_t  destip[4];
   uint16_t destport;
   uint8_t  packinfo = 0;
   switch(getSn_SR(sn))
   {
      case SOCK_UDP :
         if((size = getSn_RX_RSR(sn)) > 0) // Don't need to check SOCKERR_BUSY because it doesn't not occur.
         {
            if(size > DATA_BUF_SIZE) size = DATA_BUF_SIZE;
            ret = recvfrom(sn,buf,size,destip,(uint16_t*)&destport,&packinfo);
            if(ret <= 0)   // check SOCKERR_BUSY & SOCKERR_XXX. For showing the occurrence of SOCKERR_BUSY.
            {
               printf("%d: recvfrom error. %ld\r\n",sn,ret);
               return ret;
            }
            size = (uint16_t) ret;
            sentsize = 0;
            while(sentsize != size)
            {
               ret = sendto(sn,buf+sentsize,size-sentsize,destip,destport);
               if(ret < 0)
               {
                  printf("%d: sendto error. %ld\r\n",sn,ret);
                  return ret;
               }
               sentsize += ret; // Don't care SOCKERR_BUSY, because it is zero.
            }
         }
         break;
      case SOCK_CLOSED:
         printf("%d:LBUStart\r\n",sn);
         if((ret=socket(sn,Sn_MR_UDP,port,0x00)) != sn)
            return ret;
         printf("%d:Opened\r\n",sn);
         break;
      default :
         break;
   }
   return 1;
}
#endif

#if LOOPBACK_MODE == LOOPBACK_BLOCK_API
int32_t loopback_tcps(uint8_t sn, uint8_t* buf, uint16_t size)
{
   int32_t ret = 0;
   ret = recv(sn,buf,size);
   if(ret != size)
   {
      if(ret < 0)
      {
         printf("%d:recv() error:%ld\r\n",sn,ret);
         close(sn);
         return ret;
      }
   }
   size = ret;
   ret = send(sn,buf,size);
   if(ret != size)
   {
      if(ret < 0)
      {
         printf("%d:send() error:%ld\r\n",sn,ret);
         close(sn);
      }
   }
   return ret;
}

int32_t loopback_udps(uint8_t sn, uint8_t* buf, uint16_t size)
{
   int32_t  ret = 0;
   static uint8_t  addr[4] = {0,};
   static uint16_t port = 0;
   uint8_t  packinfo;

   if((ret = recvfrom(sn,buf,size, addr,&port,&packinfo)) < 0)
   {
      printf("%d:recvfrom error:%ld\r\n",sn,ret);
      return ret;
   }
   if(packinfo & 0x80)
   {
      printf("%d:recvfrom %d.%d.%d.%d(%d), size=%ld.\r\n",sn,addr[0],addr[1],addr[2],addr[3],port, ret);
   }
   if(packinfo & 0x01)
   {
      printf("%d:recvfrom remained packet.\r\n",sn);
   }
   else
   {
      printf("%d:recvfrom completed.\r\n",sn);
   }
   if( (ret = sendto(sn, buf, ret, addr, port)) < 0)
   {
      printf("%d:sendto error:%ld\r\n",sn,ret);
      return ret;
   }
   printf("%d:sendto %d.%d.%d.%d(%d), size=%ld\r\n",sn,addr[0],addr[1],addr[2],addr[3],port, ret);
   return ret;
}
#endif


#if LOOPBACK_MODE == LOOPBACK_NONBLOCK_API

int32_t loopback_tcps(uint8_t sn, uint8_t* buf, uint16_t port)
{
   int32_t ret = 0;
   int32_t sentsize = 0;
   uint16_t size;

   switch(getSn_SR(sn))
   {
   case SOCK_ESTABLISHED :
	   if(connection_status[sn] == 0)
	   {
		   printf("%d: Connected\r\n", sn);
		   connection_status[sn] = 1;
	   }
       if((size = getSn_RX_RSR(sn)) > 0) // Don't need to check SOCKERR_BUSY because it doesn't not occur.
       {
//       	printf("size: %d\r\n", size);

       	if(size > DATA_BUF_SIZE) size = DATA_BUF_SIZE;

//       	printf("timer: %d\r\n", GlobalCounter);
          ret = recv(sn,buf,size);
//          printf("recv count: %d\r\n", ret);
		   if(ret != size)
		   {
			  if(ret==SOCK_BUSY) return 0;
			  if(ret < 0)
			  {
				 printf("%d:recv() error:%ld\r\n",sn,ret);
				 close(sn);
		         connection_status[sn] = 0;
				 return ret;
			  }
		   }

		   size = ret;
		   sentsize = 0;
		   while(sentsize != size)
		   {
			  ret = send(sn,buf+sentsize,size-sentsize);
			  if(ret != size-sentsize)
			  {
				 if(ret < 0)
				 {
					printf("%d:send() error:%ld\r\n",sn,ret);
					close(sn);
  		            connection_status[sn] = 0;
					return ret;
				 }
			  }
			  sentsize += ret;  // Don't care SOCKERR_BUSY, because it is zero.
		   }
		   return size;
       }
	   break;
   case SOCK_CLOSE_WAIT :
       printf("%d:CloseWait\r\n",sn);
       if((ret=disconnect(sn)) != SOCK_OK) return ret;
       connection_status[sn] = 0;
       printf("%d:Closed\r\n",sn);
	   break;
   case SOCK_CLOSED :
       printf("%d:LBTStart\r\n",sn);
       if((ret=socket(sn,Sn_MR_TCP,port,SF_TCP_NODELAY)) != sn)
       {
    	   printf("%d:socket() error:%ld\r\n", sn, ret);
           connection_status[sn] = 0;
    	   close(sn);
    	   return ret;
       }
	   break;
   case SOCK_INIT :
       printf("%d:Opened\r\n",sn);
       if( (ret = listen(sn)) != SOCK_OK)
       {
           printf("%d:Listen error\r\n",sn);
           return ret;
       }
       printf("%d:Listen ok\r\n",sn);
	   break;
   default :
	   break;
   }

	 return ret;

}

//int32_t loopback_udps(uint8_t sn, uint8_t* buf, uint16_t port)
//{
//   int32_t  ret;
//   uint16_t size, sentsize;
//   uint8_t  destip[4];
//   uint16_t destport;
//   uint8_t  packinfo = 0;
//   switch(getSn_SR(sn))
//   {
//      case SOCK_UDP :
//         if((size = getSn_RX_RSR(sn)) > 0) // Don't need to check SOCKERR_BUSY because it doesn't not occur./*���յ�����*/
//         {
//            if(size > DATA_BUF_SIZE) size = DATA_BUF_SIZE;
//            ret = recvfrom(sn,buf,size,destip,(uint16_t*)&destport);
//            if(ret <= 0)   // check SOCKERR_BUSY & SOCKERR_XXX. For showing the occurrence of SOCKERR_BUSY.
//            {
//               printf("%d: recvfrom error. %ld\r\n",sn,ret);
//               return ret;
//            }
//            size = (uint16_t) ret;
//            sentsize = 0;
//            while(sentsize != size)
//            {
//               ret = sendto(sn,buf+sentsize,size-sentsize,destip,destport);
//               if(ret < 0)
//               {
//                  printf("%d: sendto error. %ld\r\n",sn,ret);
//                  return ret;
//               }
//               sentsize += ret; // Don't care SOCKERR_BUSY, because it is zero.
//            }
//         }
//         break;
//      case SOCK_CLOSED:
//         printf("%d:LBUStart\r\n",sn);
//         if((ret=socket(sn,Sn_MR_UDP,port,0x00)) != sn)
//            return ret;
//         printf("%d:Opened\r\n",sn);
//         break;
//      default :
//         break;
//   }
//   return 1;

//}

int32_t rcvonly_tcps(uint8_t sn, uint8_t* buf, uint16_t port)
{
   int32_t ret = 0;
   int32_t sentsize = 0;
   uint16_t size;

   switch(getSn_SR(sn))
   {
	case SOCK_ESTABLISHED :
	   if(connection_status[sn] == 0)
	   {
		   printf("%d: Connected\r\n", sn);
		   connection_status[sn] = 1;
	   }
       
		 if((size = getSn_RX_RSR(sn)) > 0) // Don't need to check SOCKERR_BUSY because it doesn't not occur.
     {
//       	printf("size: %d\r\n", size);

       	if(size > DATA_BUF_SIZE) size = DATA_BUF_SIZE;

//       	printf("timer: %d\r\n", GlobalCounter);
        ret = recv(sn,buf,size);
//          printf("recv count: %d\r\n", ret);
		   if(ret != size)
		   {
					if(ret==SOCK_BUSY) return 0;
					if(ret < 0)
					{
						printf("%d:recv() error:%ld\r\n",sn,ret);
						close(sn);
		        connection_status[sn] = 0;
						return ret;
					}
		   }

				return size;
      }
	   break;
   case SOCK_CLOSE_WAIT :
       printf("%d:CloseWait\r\n",sn);
       if((ret=disconnect(sn)) != SOCK_OK) return ret;
       connection_status[sn] = 0;
       printf("%d:Closed\r\n",sn);
	   break;
   case SOCK_CLOSED :
       printf("%d:ROTStart\r\n",sn);
       if((ret=socket(sn,Sn_MR_TCP,port,SF_TCP_NODELAY)) != sn)
       {
    	   printf("%d:socket() error:%ld\r\n", sn, ret);
           connection_status[sn] = 0;
    	   close(sn);
    	   return ret;
       }
	   break;
   case SOCK_INIT :
       printf("%d:Opened\r\n",sn);
       if( (ret = listen(sn)) != SOCK_OK)
       {
           printf("%d:Listen error\r\n",sn);
           return ret;
       }
       printf("%d:Listen ok\r\n",sn);
	   break;
   default :
	   break;
   }

	 return ret;

}
#endif
