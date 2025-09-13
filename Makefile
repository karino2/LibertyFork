SRC = main.c \
      genmove.c \
      attacker.c \
      defender.c \
      shapes.c \
      matchpat.c \
      hacks.c \
      gnugo.c \
      opening.c \
      sethand.c \
      showbord.c \
      gmp.c \
      gtp.c \
      play_gtp.c

OBJ = $(SRC:.c=.o)

CFLAGS = -g -O2

#liberty : patterns.h connections.h $(OBJ) mkpat 
liberty : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o liberty

#matchpat.o : patterns.h

$(OBJ) : liberty.h

patterns.h : patterns.db mkpat
	mkpat pat <patterns.db >patterns.h

connections.h : connections.db mkpat
	mkpat con <connections.db >connections.h

renum :
	mv patterns.db patterns.db.bak
	awk '{if ($$0 !~ /^# pattern/) print $0; else {printf("# pattern %d\n",i); i+=1}}' patterns.db.bak>patterns.db

mkpat : mkpat.c
	$(CC) $(CFLAGS) mkpat.c -o mkpat

clean: 
	rm -f *.o liberty mkpat

tar:
	rm liberty-1.0/*
	cp $(SRC) gmp.h liberty.h mkpat.c patterns.db patterns.h connections.db connections.h main.c newlib.h newstring.c Makefile README COPYING liberty-1.0
	tar cvf liberty-1.0.tar liberty-1.0/*
	gzip liberty-1.0.tar

untar:
	gunzip liberty-1.0.tar.gz
	tar xvf ./liberty-1.0.tar
	cp liberty-1.0/* .

todisk:
	make tar
	mcopy liberty-1.0.tar.gz a:

fromdisk:
	mcopy a:liberty-1.0.tar.gz .
	make untar

cleanup:
	make tar
	rm -rf liberty-1.0
	rm *.c *.h *.db README COPYING
