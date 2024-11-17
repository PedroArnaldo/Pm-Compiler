inteiro i;
inteiro j;

ler(i);

se i>19
  entao
    se i<30
	  entao
	    mostrar (i);
	  senao
	    mostrar(i);
  senao
    se i<10
	  entao
	    mostrar(i);
	  senao
	    mostrar(i);
mostrar (i);

repita {
  i = i + 1;
  j = 11;
} ate i == 3;

mostrar(i);
mostrar(j);