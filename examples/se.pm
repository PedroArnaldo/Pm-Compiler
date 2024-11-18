inteiro iva;
inteiro j;

ler(i);

iva = 10;

se iva>19
  entao
    se iva<30
	  entao
	    mostrar (iva);
	  senao
	    mostrar(iva);
  senao
    se iva<10
	  entao
	    mostrar(iva);
	  senao
	    mostrar(iva);
mostrar (iva);

repita {
  iva = iva + 1;
  j = 11;
} ate iva == 3;

mostrar(iva);