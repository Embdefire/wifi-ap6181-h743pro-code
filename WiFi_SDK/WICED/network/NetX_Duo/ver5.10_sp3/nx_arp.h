/**************************************************************************/
/*                                                                        */
/*            Copyright (c) 1996-2016 by Express Logic Inc.               */
/*                                                                        */
/*  This software is copyrighted by and is the sole property of Express   */
/*  Logic, Inc.  All rights, title, ownership, or other interests         */
/*  in the software remain the property of Express Logic, Inc.  This      */
/*  software may only be used in accordance with the corresponding        */
/*  license agreement.  Any unauthorized use, duplication, transmission,  */
/*  distribution, or disclosure of this software is expressly forbidden.  */
/*                                                                        */
/*  This Copyright notice may not be removed or modified without prior    */
/*  written consent of Express Logic, Inc.                                */
/*                                                                        */
/*  Express Logic, Inc. reserves the right to modify this software        */
/*  without notice.                                                       */
/*                                                                        */
/*  Express Logic, Inc.                     info@expresslogic.com         */
/*  11423 West Bernardo Court               http://www.expresslogic.com   */
/*  San Diego, CA  92127                                                  */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** NetX Component                                                        */
/**                                                                       */
/**   Address Resolution Protocol (ARP)                                   */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    nx_arp.h                                            PORTABLE C      */
/*                                                           5.10         */
/*  AUTHOR                                                                */
/*                                                                        */
/*    William E. Lamie, Express Logic, Inc.                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the NetX Address Resolution Protocol component,   */
/*    including all data types and external references.  It is assumed    */
/*    that nx_api.h and nx_port.h have already been included.             */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-12-2005     William E. Lamie         Initial Version 5.0           */
/*  08-09-2007     William E. Lamie         Modified comment(s), and      */
/*                                            changed UL to ULONG cast,   */
/*                                            resulting in version 5.1    */
/*  12-30-2007     Yuxin Zhou               Modified comment(s),          */
/*                                            resulting in version 5.2    */
/*  08-03-2009     William E. Lamie         Modified comment(s),          */
/*                                            resulting in version 5.3    */
/*  11-23-2009     Yuxin Zhou               Modified comment(s),          */
/*                                            resulting in version 5.4    */
/*  06-01-2010     Yuxin Zhou               Removed internal debug logic, */
/*                                            resulting in version 5.5    */
/*  10-10-2011     Yuxin Zhou               Modified comment(s),  added   */
/*                                            interface paramter to       */
/*                                            _nx_arp_packet_send call,   */
/*                                            resulting in version 5.6    */
/*  01-31-2013     Yuxin Zhou               Modified comment(s),          */
/*                                            resulting in version 5.7    */
/*  01-12-2015     Yuxin Zhou               Modified comment(s), and      */
/*                                            added new API definitions,  */
/*                                            added ARP defend feature,   */
/*                                            resulting in version 5.8    */
/*  02-22-2016     Yuxin Zhou               Modified comment(s), added    */
/*                                            new API definition and the  */
/*                                            is_static parameter to      */
/*                                            _nx_arp_entry_allocate,     */
/*                                            resulting in version 5.9    */
/*  05-10-2016     Yuxin Zhou               Modified comment(s), added    */
/*                                            definitions for sending ARP */
/*                                            probe and announce,         */
/*                                            resulting in version 5.10   */
/*                                                                        */
/**************************************************************************/

#ifndef NX_ARP_H
#define NX_ARP_H


/* Define ARP Message format.  This will get encapsulated by an Ethernet frame
   as well.  The Ethernet frame will typically have a 6-byte Ethernet destination
   address, a 6-byte Ethernet source address, and a 2-byte Ethernet Frame type,
   which is 0x0806.  Regular IP frames have a frame type of 0x0800.

    Byte offset     Size            Meaning

        0           2           Hardware type (1 for Ethernet)
        2           2           Protocol type (0x0800 for IP)
        4           1           Number of bytes for hardware address (6 for Ethernet)
        5           1           Number of bytes for IP address (4 for IP)
        6           2           Operation, ARP request is 1, ARP reply is 2
        8           6           Sender's Ethernet Address
        14          4           Sender's IP Address
        18          6           Target Ethernet Address
        24          4           Target IP Address
 */

#define NX_ARP_HARDWARE_TYPE   ((ULONG)0x0001)
#define NX_ARP_PROTOCOL_TYPE   ((ULONG)0x0800)
#define NX_ARP_HARDWARE_SIZE   ((ULONG)0x06)
#define NX_ARP_PROTOCOL_SIZE   ((ULONG)0x04)
#define NX_ARP_OPTION_REQUEST  ((ULONG)0x0001)
#define NX_ARP_OPTION_RESPONSE ((ULONG)0x0002)
#define NX_ARP_MESSAGE_SIZE    28


/* Define the ARP defend interval. The default value is 10 seconds. */
#ifndef NX_ARP_DEFEND_INTERVAL

#define NX_ARP_DEFEND_INTERVAL 10

#endif /* NX_ARP_DEFEND_INTERVAL */


/* Define ARP function prototypes.  */

VOID _nx_arp_initialize(VOID);
UINT _nx_arp_dynamic_entries_invalidate(NX_IP *ip_ptr);
UINT _nx_arp_dynamic_entry_set(NX_IP *ip_ptr, ULONG ip_address, ULONG physical_msw, ULONG physical_lsw);
UINT _nx_arp_dynamic_entry_delete(NX_IP *ip_ptr, NX_ARP *arp_ptr);
VOID _nx_arp_static_entry_delete_internal(NX_IP *ip_ptr, NX_ARP *arp_entry);
UINT _nx_arp_enable(NX_IP *ip_ptr, VOID *arp_cache_memory, ULONG arp_cache_size);
UINT _nx_arp_entry_delete(NX_IP *ip_ptr, ULONG ip_address);
UINT _nx_arp_gratuitous_send(NX_IP *ip_ptr, VOID (*response_handler)(NX_IP *ip_ptr, NX_PACKET *packet_ptr));
UINT _nx_arp_hardware_address_find(NX_IP *ip_ptr, ULONG ip_address,  ULONG *physical_msw, ULONG *physical_lsw);
UINT _nx_arp_info_get(NX_IP *ip_ptr, ULONG *arp_requests_sent, ULONG *arp_requests_received,
                      ULONG *arp_responses_sent, ULONG *arp_responses_received,
                      ULONG *arp_dynamic_entries, ULONG *arp_static_entries,
                      ULONG *arp_aged_entries, ULONG *arp_invalid_messages);
UINT _nx_arp_ip_address_find(NX_IP *ip_ptr, ULONG *ip_address, ULONG physical_msw, ULONG physical_lsw);
VOID _nx_arp_queue_process(NX_IP *ip_ptr);
VOID _nx_arp_queue_send(NX_IP *ip_ptr, NX_ARP *arp_ptr);
UINT _nx_arp_static_entries_delete(NX_IP *ip_ptr);
UINT _nx_arp_static_entry_create(NX_IP *ip_ptr, ULONG ip_address,  ULONG physical_msw, ULONG physical_lsw);
UINT _nx_arp_static_entry_delete(NX_IP *ip_ptr, ULONG ip_address,  ULONG physical_msw, ULONG physical_lsw);
UINT _nx_arp_entry_allocate(NX_IP *ip_ptr, NX_ARP **arp_ptr, UINT is_static);
/*lint -sem(_nx_arp_packet_send, 3p) nx_interface must not be NULL.  */
VOID _nx_arp_packet_send(NX_IP *ip_ptr, ULONG destination_ip, NX_INTERFACE *nx_interface);
VOID _nx_arp_packet_receive(NX_IP *ip_ptr, NX_PACKET *packet_ptr);
VOID _nx_arp_packet_deferred_receive(NX_IP *ip_ptr, NX_PACKET *packet_ptr);
VOID _nx_arp_periodic_update(NX_IP *ip_ptr);
UINT _nx_arp_interface_entries_delete(NX_IP *ip_ptr, UINT index);
UINT _nx_arp_probe_send(NX_IP *ip_ptr, UINT interface_index, ULONG probe_address);
UINT _nx_arp_announce_send(NX_IP *ip_ptr, UINT interface_index, ULONG announce_address);

/* Define error checking shells for ARP services.  These are only referenced by the
   application.  */

UINT _nxe_arp_dynamic_entries_invalidate(NX_IP *ip_ptr);
UINT _nxe_arp_dynamic_entry_set(NX_IP *ip_ptr, ULONG ip_address, ULONG physical_msw, ULONG physical_lsw);
UINT _nxe_arp_enable(NX_IP *ip_ptr, VOID *arp_cache_memory, ULONG arp_cache_size);
UINT _nxe_arp_entry_delete(NX_IP *ip_ptr, ULONG ip_address);
UINT _nxe_arp_gratuitous_send(NX_IP *ip_ptr, VOID (*response_handler)(NX_IP *ip_ptr, NX_PACKET *packet_ptr));
UINT _nxe_arp_hardware_address_find(NX_IP *ip_ptr, ULONG ip_address, ULONG *physical_msw, ULONG *physical_lsw);
UINT _nxe_arp_info_get(NX_IP *ip_ptr, ULONG *arp_requests_sent, ULONG *arp_requests_received,
                       ULONG *arp_responses_sent, ULONG *arp_responses_received,
                       ULONG *arp_dynamic_entries, ULONG *arp_static_entries,
                       ULONG *arp_aged_entries, ULONG *arp_invalid_messages);
UINT _nxe_arp_ip_address_find(NX_IP *ip_ptr, ULONG *ip_address, ULONG physical_msw, ULONG physical_lsw);
UINT _nxe_arp_static_entries_delete(NX_IP *ip_ptr);
UINT _nxe_arp_static_entry_create(NX_IP *ip_ptr, ULONG ip_address,  ULONG physical_msw, ULONG physical_lsw);
UINT _nxe_arp_static_entry_delete(NX_IP *ip_ptr, ULONG ip_address, ULONG physical_msw, ULONG physical_lsw);
#endif

