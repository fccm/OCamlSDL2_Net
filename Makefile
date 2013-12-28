# OCamlSDL2_Net - An OCaml interface to the SDL2_Net library
# Copyright (C) 2013 Florent Monnier
#
# This software is provided "AS-IS", without any express or implied warranty.
# In no event will the authors be held liable for any damages arising from
# the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it freely.

OCAML = ocaml
OCAMLC = ocamlc
OCAMLOPT = ocamlopt
OCAMLDOC = ocamldoc
OCAMLDEP = ocamldep
OCAMLFIND = ocamlfind
OCAMLMKLIB = ocamlmklib
CC = gcc
OCAMLDIR = $(shell $(OCAMLC) -where)
DIRSEP = $(shell $(OCAML) dir_sep.ml)

PKG_CONFIG = pkg-config
CFLAGS = $(shell $(PKG_CONFIG) --cflags SDL2_net)
CFLAGS += -I$(OCAMLDIR)/sdl2/include/
LIBS_ = $(shell $(PKG_CONFIG) --libs SDL2_net)
LIBS = $(shell $(OCAML) prm.ml $(LIBS_))

BINDINGS_DIR = sdl2_net
EDITOR = vim

.PHONY: all lib opt
all: lib opt
lib: sdl2_net.cma
opt: sdl2_net.cmxa

%.cmi: %.mli
	$(OCAMLC) -c $<

%.cmo: %.ml
	$(OCAMLC) -c -I +sdl2 $<

%.cmx: %.ml
	$(OCAMLOPT) -c -I +sdl2 $<

sdlnet_stub.o: sdlnet_stub.c
	$(OCAMLC) -ccopt "-static $(CFLAGS) -g -O " $<


sdl2_net.cma: sdlnet.cmo libsdlstub.a
	$(OCAMLMKLIB) -o sdl2_net -oc sdl2_stubs sdlnet.cmo $(LIBS)

sdl2_net.cmxa: sdlnet.cmx libsdlstub.a
	$(OCAMLMKLIB) -o sdl2_net -oc sdl2_stubs sdlnet.cmx $(LIBS)

libsdlstub.a: sdlnet_stub.o
	$(OCAMLMKLIB) -o sdl2_net -oc sdl2_stubs sdlnet_stub.o $(LIBS)

.PHONY: edit
edit:
	$(EDITOR) sdlnet.ml sdlnet_stub.c

.PHONY: clean
clean:
	$(RM) *.[oas] *.cm[ioxta] *.cmx[as] *.so *.dll *.opt *.exe

