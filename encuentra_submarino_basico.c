/*
|---------------------------------------------------------------------------------------------------
| SEGUNDO EXAMEN PARCIAL LAPSO 2016-1 / Encuentra el submarino
|---------------------------------------------------------------------------------------------------
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void mostrar_mapa(char mapa[][15], int N, int emb_restantes, int intentos);

int main()
{
	int  N, emb_restantes, i, j, M, cont_buques, cont_lanchas, cont_submarinos, k,
	     band, estado, intento, celdas_seleccionadas[225], aux, celda, num, v1, v2, pos,
		 cerca_buq, cerca_lan, cerca_sub, medio_buq, medio_lan, medio_sub, lejos_buq, lejos_lan, 
		 lejos_sub,total;
	char mapa[15][15], msj_error[100];
	
	// Ajustar tamaño de la ventana
	system("MODE CON:cols=100 lines=40");  
	
	// 1. Solicitar el valor de N, se debe validar que el valor se encuentre en el rango permitido
	do{		
		printf("\n Ingrese el valor de N: ");
		scanf("%d", &N);

		if(N < 5  ||  N > 15)
			printf("\n Error, el valor debe ser minimo 5 - maximo 15 \n\n");

	}while(N < 5  ||  N > 15);
	
	// Redimensionar tamaño de la ventana de acuerdo al valor de N
	if(N>=10) system("MODE CON:cols=110 lines=50");
	if(N>=13) system("MODE CON:cols=110 lines=59");

	//----------------------------------------------------------------------------------------------	
	// Se ubican en el mapa N embarcaciones de forma aleatoria, incluyendo M submarinos
		
	// Número de embarcaciones
	emb_restantes = N;
	
	// Inicializar el mapa
	for(i=0; i<N; i++)
	{
		for(j=0; j<N; j++)
		{
			mapa[i][j] = '\0';
		}
	}

	// Numero de submarinos del juego
	M = N - 4;
	
	// Ubicar las embarcaciones en el mapa (submarinos, buques y lanchas)		
	srand(time(NULL));
	
	cont_buques = cont_lanchas = 0;  cont_submarinos = M;
	
	for(k=1; k<=N; k++)
	{
		do{
			band = 0; // Celda vacía
			
			i = rand() % N;
			j = rand() % N;
			
			if(mapa[i][j] == '\0')
			{
				if(k <= M)
				{
					mapa[i][j] = 's';      // ubicar submarino
				}
				else
				{
					if(rand() % 2 == 0)
					{
						mapa[i][j] = 'b';  // ubicar buque 
						cont_buques++;
					}
					else
					{
						mapa[i][j] = 'l';  // ubicar lancha
						cont_lanchas++;
					}					
				}
			}
			else
			{
				band = 1; // Celda ocupada por otra embarcación
			}
			
		}while(band == 1);
	}

	//------------------------------------------------------------------------------------------
	estado  = 0;  // 0 = Normal   /   1 = Ganó   /   2 = Perdió	
	intento = k = 0;
		 
	// Inicializar vector de celdas seleccionadas por el usuario	
	for(i=0; i < N * N; i++)
	{
		celdas_seleccionadas[i] = 0;
	}
	
	do{
			aux = (N * 2) - intento; // Número de intentos restantes
			
			//--------------------------------------------------------------------------------------
			// Seleccionar celda
			do{
				mostrar_mapa(mapa, N, emb_restantes, aux);
				
				band = 0; 
				
				printf("\n Seleccione una celda: ");
				scanf("%d", &celda);
				
				if(celda < 1  ||  celda > N * N)
				{
					band = 1;
					printf("\n Error, celda invalida \n\n"); 
					getch();
				}	
				
				for(i=0; i <= k; i++)
				{			
					if(celda == celdas_seleccionadas[i]   &&   celda != 0)
					{
						band = 2;
						printf("\n Error, celda no disponible \n\n"); 
						getch();						
						break;	
					}	
				}				
			}while(band != 0);
			
			celdas_seleccionadas[k++] = celda;

			//--------------------------------------------------------------------------------------
			// Determinar si la celda está ocupada o no
			
			num = 1; // Contador de celdas - Iniciar desde celda número 1
			
			for(i=0; i<N; i++)
			{
				for(j=0; j<N && estado!=2; j++)
				{
					if(celda == num)
					{
						aux = ((N * 2) - intento ) - 1;
						
						if(mapa[i][j] == 'b'  ||  mapa[i][j] == 'l' || mapa[i][j] == 's')
						{
							// Si la celda está ocupada por un buque o una lancha o un submarino
														
							if(mapa[i][j] == 'b') 
							{
								cont_buques--;
								strcpy(msj_error, "\n Buque destruido \n\n");
							}
							if(mapa[i][j] == 'l')
							{
								cont_lanchas--;
								strcpy(msj_error, "\n Lancha destruida \n\n");
							} 
							if(mapa[i][j] == 's')
							{
								cont_submarinos--;
								
								if(cont_lanchas > 0   ||   cont_buques > 0)
								{
									aux    = 0;  	
									estado = 2; // Perdió
									
									strcpy(msj_error, "\n HAS PERDIDO! \n\n No destruiste todos los buques y lanchas");
								}
								else					
									strcpy(msj_error, "\n\n Submarino destruido\n\n");
							}
							
							mapa[i][j] = toupper(mapa[i][j]);
														
							mostrar_mapa(mapa, N, cont_submarinos + cont_lanchas + cont_buques, aux);
							
							printf("%s", msj_error);							
							getch();					
						}
						else
						{
							// Si la celda no está ocupada, verificar embarcaciones cercanas						
							mapa[i][j] = ' ';
							
							mostrar_mapa(mapa, N, cont_submarinos + cont_lanchas + cont_buques, aux);
							
							printf("\n\n La celda %d no esta ocupada \n\n\n", celda);
							
							// Verificar embarcaciones en la misma fila/columna
							
							for(v1=0; v1<2; v1++)
							{
								pos = (v1 == 0) ? j : i; // v1=0 (Recorrido fila) / v1=1 (Recorrido columna)
								
								// Inicializar contadores de buques, lanchas y submarinos
								cerca_buq = cerca_lan = cerca_sub = 0;
								medio_buq = medio_lan = medio_sub = 0;
								lejos_buq = lejos_lan = lejos_sub = 0;
								
								for(v2=0; v2<N; v2++)
								{
									// Cerca (menos de 3 celdas de separación) 
									if(v2 >= pos-2   &&   v2 <= pos+2) 
									{
										if(v1 == 0  &&  mapa[i][v2] == 'b') cerca_buq++;
										if(v1 == 0  &&  mapa[i][v2] == 'l') cerca_lan++;
										if(v1 == 0  &&  mapa[i][v2] == 's') cerca_sub++;
										
										if(v1 == 1  &&  mapa[v2][j] == 'b') cerca_buq++;
										if(v1 == 1  &&  mapa[v2][j] == 'l') cerca_lan++;
										if(v1 == 1  &&  mapa[v2][j] == 's') cerca_sub++;
									}
									// Medianamente cerca
									if( (v2>=pos-6  &&  v2<=pos-3)   ||   (v2>=pos+3  &&  v2<=pos+6) )
									{
										if(v1 == 0  &&  mapa[i][v2] == 'b') medio_buq++;
										if(v1 == 0  &&  mapa[i][v2] == 'l') medio_lan++;
										if(v1 == 0  &&  mapa[i][v2] == 's') medio_sub++;
										
										if(v1 == 1  &&  mapa[v2][j] == 'b') medio_buq++;
										if(v1 == 1  &&  mapa[v2][j] == 'l') medio_lan++;
										if(v1 == 1  &&  mapa[v2][j] == 's') medio_sub++;									
									} 
									// Lejos
									if(v2 < pos-6   ||   v2 > pos+6)
									{
										if(v1 == 0  &&  mapa[i][v2] == 'b') lejos_buq++;
										if(v1 == 0  &&  mapa[i][v2] == 'l') lejos_lan++;
										if(v1 == 0  &&  mapa[i][v2] == 's') lejos_sub++;
										
										if(v1 == 1  &&  mapa[v2][j] == 'b') lejos_buq++;
										if(v1 == 1  &&  mapa[v2][j] == 'l') lejos_lan++;
										if(v1 == 1  &&  mapa[v2][j] == 's') lejos_sub++;	
									}
								}
								
								total = cerca_buq + cerca_lan + cerca_sub + medio_buq + medio_lan + medio_sub + lejos_buq + lejos_lan + lejos_sub;

								if( total  >  0 )
								{
									printf("\n Embarcaciones en la misma %s", (v1 == 0) ? "fila" : "columna");
									
									if(cerca_buq > 0  ||  cerca_lan >  0  ||  cerca_sub > 0)
									{
										printf("\n\n Cerca: ");
										
										if(cerca_buq > 0) printf(" %d buque%c", cerca_buq, cerca_buq == 1 ? ' ' : 's');
										if(cerca_lan > 0) printf(" %d lancha%c", cerca_lan, cerca_lan == 1 ? ' ' : 's');
										if(cerca_sub > 0) printf(" %d submarino%c", cerca_sub, cerca_sub == 1 ? ' ' : 's');
									}
									
									if(medio_buq > 0  ||  medio_lan >  0  ||  medio_sub > 0)
									{
										printf("\n\n Medianamente cerca: ");
										
										if(medio_buq > 0) printf(" %d buque%c", medio_buq, medio_buq == 1 ? ' ' : 's');
										if(medio_lan > 0) printf(" %d lancha%c", medio_lan, medio_lan == 1 ? ' ' : 's');
										if(medio_sub > 0) printf(" %d submarino%c", medio_sub, medio_sub == 1 ? ' ' : 's');
									}
									
									if(lejos_buq > 0  ||  lejos_lan >  0  ||  lejos_sub > 0)
									{
										printf("\n\n Lejos: ");
										
										if(lejos_buq > 0) printf(" %d buque%c", lejos_buq, lejos_buq == 1 ? ' ' : 's');
										if(lejos_lan > 0) printf(" %d lancha%c", lejos_lan, lejos_lan == 1 ? ' ' : 's');
										if(lejos_sub > 0) printf(" %d submarino%c", lejos_sub, lejos_sub == 1 ? ' ' : 's');
									}										
									printf("\n\n\n");												
								}
							}															
							
							getch();
						}
					}
					
					num++; 
				}
			}			

			emb_restantes = cont_buques + cont_lanchas + cont_submarinos;
			
			if(emb_restantes == 0)
			{
				estado = 1; // Ganó
				
				printf("\n HAS GANADO! Felicitaciones \n\n");
				getch();
			}
			
			intento++;			
		
	}while(intento < N*2   &&   estado == 0);
			
	
	if(emb_restantes > 0   &&   estado == 0)
	{
		mostrar_mapa(mapa, N, emb_restantes, 0);

		printf("\n\n HAS PERDIDO! \n\n Numero de intentos agotados\n\n");
		
		getch();	
	}	
	
	return 0;
}

/** Esta función imprime en pantalla el mapa con las celdas seleccionables y las embarcaciones
    encontradas, además del encabezado del programa.
    @param  mapa           matriz de caracteres donde se ubican las embarcaciones disponibles
    @param  N              cantidad de filas y columnas de la matriz
    @param  emb_restantes  cantidad de embarcaciones por encontrar en el mapa
    @param  intentos       número de intentos restantes
*/
void mostrar_mapa(char mapa[][15], int N, int emb_restantes, int intentos)
{
	int  i, j, num = 1;
	
	system("cls");	
	printf("\n *************************** ENCUENTRA EL SUBMARINO *************************** \n\n");		
	printf("\n Numero de embarcaciones:  %d \t\t  Numero de intentos restantes:  %d   \n\n\n", 
	       emb_restantes, intentos);	
		  
	for(i=0; i<N; i++)
	{
		for(j=0; j<N; j++)
		{			
			if(mapa[i][j] == 'B'  ||  mapa[i][j]=='L'  ||  mapa[i][j]=='S'  ||  mapa[i][j]==' ')
			{
				// Celda ocupada				
				printf("%5c", mapa[i][j]);		
			}
			else
			{
				// Celda disponible
				printf("%5d", num);				
			}
				
			if(j < N-1) printf(" |");
			
			num++;
		}
		printf("\n\n");
	}	
}
