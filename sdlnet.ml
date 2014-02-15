(* OCamlSDL2_Net - An OCaml interface to the SDL2_Net library
 Copyright (C) 2013 Florent Monnier
 
 This software is provided "AS-IS", without any express or implied warranty.
 In no event will the authors be held liable for any damages arising from
 the use of this software.
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it freely.
*)
(* Network library for SDL *)

external init : unit -> unit
  = "caml_SDLNet_Init"

external quit : unit -> unit
  = "caml_SDLNet_Quit"

external linked_version : unit -> int * int * int
  = "caml_SDLNet_Linked_Version"

external compile_version : unit -> int * int * int
  = "caml_SDL_NET_VERSION"

(** {3 UDP} *)

type udp_socket

type uint16 = int

external udp_open : port:uint16 -> udp_socket
  = "ml_SDLNet_UDP_Open"

external udp_close : sock:udp_socket -> unit
  = "caml_SDLNet_UDP_Close"

type udp_packet

external alloc_packet : size:int -> udp_packet
  = "caml_SDLNet_AllocPacket"

external free_packet : packet:udp_packet -> unit
  = "caml_SDLNet_FreePacket"

type udp_packet_contents = {
  channel: int;   (* The src/dst channel of the packet *)
  data: string;   (* The packet data *)
  maxlen: int;    (* The size of the data buffer *)
  status: int;    (* packet status after sending *)
  address_host: int32;
                  (* The source/dest address of an incoming/outgoing packet *)
  address_port: int;
}

external udp_recv :
  sock:udp_socket -> packet:udp_packet -> udp_packet_contents option
  = "caml_SDLNet_UDP_Recv"

external udp_packet_set_address :
  packet:udp_packet -> host:int * int * int * int -> port:int -> unit
  = "caml_UDPpacket_set_address"

external udp_packet_set_data :
  packet:udp_packet -> data:string -> unit
  = "caml_UDPpacket_set_data"

external udp_packet_get_len : packet:udp_packet -> int
  = "caml_UDPpacket_get_len"

external udp_packet_get_maxlen : packet:udp_packet -> int
  = "caml_UDPpacket_get_maxlen"

external get_max_udp_channels : unit -> int
  = "caml_get_SDLNet_Max_UDPChannels"

external get_max_udp_addresses : unit -> int
  = "caml_get_SDLNet_Max_UDPAddresses"

external udp_send : sock:udp_socket -> channel:int -> packet:udp_packet -> int
  = "caml_SDLNet_UDP_Send"

external udp_unbind : sock:udp_socket -> channel:int -> unit
  = "caml_SDLNet_UDP_Unbind"

