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

