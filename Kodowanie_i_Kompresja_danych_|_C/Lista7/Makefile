all: koder szum dekoder sprawdz

koder: koder.c
	gcc -std=c99 -o koder koder.c

szum: szum.c
	gcc -std=c99 -o szum szum.c 

dekoder: dekoder.c
	gcc -std=c99 -o dekoder dekoder.c


sprawdz: sprawdz.c
	gcc -std=c99 -o sprawdz sprawdz.c

$(info )
$(info )
$(info |                         USAGE:                              |)
$(info ---------------------------------------------------------------)
$(info | koder   |  ./koder <in_file> <out_file>                     |)
$(info |---------|---------------------------------------------------|)
$(info | szum    |  ./szum <prawdopodobienstwo> <in_file> <out_file> |)
$(info |---------|---------------------------------------------------|)
$(info | dekoder |  ./dekoder <in_file> <out_file>                   |)
$(info |---------|---------------------------------------------------|)
$(info | sprawdz |  ./sprawdz <first in_file> <second in_file>       |)
$(info L--------------------------------------------------------------)
$(info )
$(info )

