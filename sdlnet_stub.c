/* OCamlSDL2_Net - An OCaml interface to the SDL2_Net library
 Copyright (C) 2013 Florent Monnier
 
 This software is provided "AS-IS", without any express or implied warranty.
 In no event will the authors be held liable for any damages arising from
 the use of this software.
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it freely.
*/
#define CAML_NAME_SPACE
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/fail.h>

#include <SDL_net.h>

#define OCSDLNET_CHECK_U16SIZE 1
#ifdef OCSDLNET_CHECK_U16SIZE

static inline Uint16 Uint16_val(value v)
{
  long d = Long_val(v);
  if (d < 0 || d > 0xFFFF) caml_invalid_argument("uint16");
  return ((Uint16) d);
}

#else /* OCSDLNET_CHECK_U16SIZE */

#define Uint16_val(v) ((Uint16) Long_val(v))

#endif /* OCSDLNET_CHECK_U16SIZE */

#define Val_none Val_int(0)

#define Val_UDPsocket(udpsock) ((value)(udpsock))
#define UDPsocket_val(udpsock) ((UDPsocket)(udpsock))

#define Val_UDPpacket(packet) ((value)(packet))
#define UDPpacket_val(packet) ((UDPpacket *)(packet))

static value
Val_some(value v)
{   
    CAMLparam1(v);
    CAMLlocal1(some);
    some = caml_alloc(1, 0);
    Store_field(some, 0, v);
    CAMLreturn(some);
}

CAMLprim value
caml_SDLNet_Init(value unit)
{
    int r = SDLNet_Init();
    if (r) caml_failwith("Sdlnet.init");
    return Val_unit;
}

CAMLprim value
caml_SDLNet_Quit(value unit)
{
    SDLNet_Quit();
    return Val_unit;
}

CAMLprim value
caml_SDLNet_Linked_Version(value unit)
{
  CAMLparam1(unit);
  CAMLlocal1(ver);

  const SDL_version *link_version = SDLNet_Linked_Version();

  ver = caml_alloc(3, 0);
  Store_field(ver, 0, Val_int(link_version->major));
  Store_field(ver, 1, Val_int(link_version->minor));
  Store_field(ver, 2, Val_int(link_version->patch));
  CAMLreturn(ver);
}

CAMLprim value
caml_SDL_NET_VERSION(value unit)
{
    CAMLparam1(unit);
    CAMLlocal1(ver);

    SDL_version compile_version;
    SDL_NET_VERSION(&compile_version);

    ver = caml_alloc(3, 0);
    Store_field(ver, 0, Val_int(compile_version.major));
    Store_field(ver, 1, Val_int(compile_version.minor));
    Store_field(ver, 2, Val_int(compile_version.patch));
    CAMLreturn(ver);
}

CAMLprim value
caml_SDLNet_UDP_Open(value port)
{
    UDPsocket udpsock;
    udpsock = SDLNet_UDP_Open(Uint16_val(port));
    if (!udpsock) {
        caml_failwith(SDLNet_GetError());
    }
    return Val_UDPsocket(udpsock);
}

CAMLprim value
caml_SDLNet_UDP_Close(value udpsock)
{
    SDLNet_UDP_Close(UDPsocket_val(udpsock));
    return Val_unit;
}

CAMLprim value
caml_SDLNet_AllocPacket(value size)
{
    UDPpacket *packet;
    packet = SDLNet_AllocPacket(Int_val(size));
    if (!packet) {
        caml_failwith(SDLNet_GetError());
    }
    return Val_UDPpacket(packet);
}

CAMLprim value 
caml_SDLNet_FreePacket(value packet)
{
    SDLNet_FreePacket(UDPpacket_val(packet));
    return Val_unit;
}

CAMLprim value
caml_UDPpacket_set_address(value packet, value host, value port)
{
    UDPpacket *p = UDPpacket_val(packet);
    Uint32 h;
    h =  ((Uint32) Long_val(Field(host, 0)));
    h |= ((Uint32) Long_val(Field(host, 1))) << 8;
    h |= ((Uint32) Long_val(Field(host, 2))) << 16;
    h |= ((Uint32) Long_val(Field(host, 3))) << 24;
    p->address.host = h;
    p->address.port = Int_val(port);
    return Val_unit;
}

CAMLprim value
caml_UDPpacket_set_data(value packet, value buf)
{
    UDPpacket *p = UDPpacket_val(packet);
    p->len = caml_string_length(buf) + 1;
    memcpy(p->data, String_val(buf), p->len);
    return Val_unit;
}

CAMLprim value
caml_UDPpacket_get_len(value packet)
{
    UDPpacket *p = UDPpacket_val(packet);
    return Val_int(p->len);
}

CAMLprim value
caml_UDPpacket_get_maxlen(value packet)
{
    UDPpacket *p = UDPpacket_val(packet);
    return Val_int(p->maxlen);
}

CAMLprim value
caml_get_SDLNet_Max_UDPChannels(value unit)
{
    return Val_int(SDLNET_MAX_UDPCHANNELS);
}

CAMLprim value
caml_get_SDLNet_Max_UDPAddresses(value unit)
{
    return Val_int(SDLNET_MAX_UDPADDRESSES);
}

CAMLprim value
caml_SDLNet_UDP_Recv(value sock, value packet)
{
    CAMLparam2(sock, packet);
    CAMLlocal3(res, pc, buf);
    
    UDPpacket *p = UDPpacket_val(packet);
    int ret = SDLNet_UDP_Recv(UDPsocket_val(sock), p);
    if (ret == -1) {
        caml_failwith(SDLNet_GetError());
    }
    if (ret == 0) {
        res = Val_none;
    }
    if (ret == 1) {
        pc = caml_alloc(6, 0);
        buf = caml_alloc_string(p->len-1);
        memcpy(String_val(buf), p->data, p->len-1);
        
        Store_field(pc, 0, Val_int(p->channel));
        Store_field(pc, 1, buf);
        Store_field(pc, 2, Val_int(p->maxlen));
        Store_field(pc, 3, Val_int(p->status));
        Store_field(pc, 4, caml_copy_int32(p->address.host));
        Store_field(pc, 5, Val_int(p->address.port));
        
        res = Val_some(pc);
    }
    CAMLreturn(res);
}

CAMLprim value
caml_SDLNet_UDP_Send(value sock, value channel, value packet)
{
    int numsent = SDLNet_UDP_Send(
          UDPsocket_val(sock), Int_val(channel), UDPpacket_val(packet));
    if (!numsent) {
      caml_failwith(SDLNet_GetError());
    }
    return Val_int(numsent);
}

CAMLprim value
caml_SDLNet_UDP_Unbind(value sock, value channel)
{
    SDLNet_UDP_Unbind((UDPsocket) sock, Int_val(channel));
    return Val_unit;
}

static value
value_of_IPaddress(IPaddress * address)
{
    CAMLparam0();
    CAMLlocal2(addr, h);
    
    h = caml_alloc(4, 0);
    
    Store_field(h, 0, Val_int((address->host & 0xFF)));
    Store_field(h, 1, Val_int((address->host >>  8) & 0xFF));
    Store_field(h, 2, Val_int((address->host >> 16) & 0xFF));
    Store_field(h, 3, Val_int((address->host >> 24) & 0xFF));
    
    addr = caml_alloc(2, 0);
    Store_field(addr, 0, h);
    Store_field(addr, 1, Val_int(address->port));
    CAMLreturn(addr);
}

CAMLprim value
caml_SDLNet_ResolveHost(value host, value port)
{
    IPaddress address;
    int ret = SDLNet_ResolveHost(&address, String_val(host), Int_val(port));
    if (ret == -1) {
      caml_failwith(SDLNet_GetError());
    }
    
    return value_of_IPaddress(&address);
}

static inline void
IPaddress_of_value(
        value ip_address,
        IPaddress *ip,
        const char *f_name)
{
    Uint32 h0, h1, h2, h3;
    
    value host = Field(ip_address, 0);
    value port = Field(ip_address, 1);
    
    h0 = (Uint32) Long_val(Field(host, 0));
    h1 = (Uint32) Long_val(Field(host, 1));
    h2 = (Uint32) Long_val(Field(host, 2));
    h3 = (Uint32) Long_val(Field(host, 3));
    
    if ((h0 | 0xFF) != 0xFF) caml_invalid_argument(f_name);
    if ((h1 | 0xFF) != 0xFF) caml_invalid_argument(f_name);
    if ((h2 | 0xFF) != 0xFF) caml_invalid_argument(f_name);
    if ((h3 | 0xFF) != 0xFF) caml_invalid_argument(f_name);
    
    ip->host = h0 | (h1 << 8) | (h2 << 16) | (h3 << 24);
    ip->port = Int_val(port);
}

CAMLprim value
caml_SDLNet_ResolveIP(value ip_addr)
{
    CAMLparam1(ip_addr);
    IPaddress ip;
    IPaddress_of_value(ip_addr, &ip, "Sdlnet.resolve_ip");
    
    const char *host = SDLNet_ResolveIP(&ip);
    
    CAMLreturn(caml_copy_string(host));
}

/* vim: set ts=4 sw=4 et: */
