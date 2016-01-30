#ifndef CFGVIEW_H_
#define CFGVIEW_H_

	//
	void cfgView_DatosConfig(char * IP_DEL_PROCESO, int Puerto_ESCUCHA, int Cant_Marcos, int Tamanio_Marco, int Max_Marcos_Por_Proceso, double Retardo, char* Algoritmo_De_Memoria);

	//
	void cfgView_DatosConfigSwap(char * IP_SWAP, int Puerto_SWAP);

	//
	void cfgView_DatosConfigOpcionales(char * TLB_Habilitada, int Entradas_TLB);

	//
	void cfgView_HeaderDatosConfig();
#endif
