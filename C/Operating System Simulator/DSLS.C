/*--------------------------------------------------------------------
            INSTITUTO TECNOLOGICO de morelia
          ACADEMIA DE SISTEMAS COMPUTACIONALES
               C¢digo Gen‚rico


M¢dulo    : Listas Ligadas, simples y dobles
Archivo   : DSLS.C
Objetivo  : Librer¡a para construir y manejar listas ligadas,
         simples o dobles, de manera gen‚rica

Dise¤o    : M. en C. Felipe Morales L¢pez

Implemento: Morales L¢pez Felipe

Versi¢n   : Enero/93
--------------------------------------------------------------------*/

#include "DSLS.H"
/*-------------------------------------------------------------------
funci¢n   : break_list_ls
objetivo  : Divide la lista en dos, la nueva lista inicia en el nodo
         dado como referencia.
par metros: lista = direcci¢n de la cabecera de la lista a dividir
         nodo  = direcci¢n del nodo de referencia.
         lis2  = direccion de la cabecera de la lista destino
         tipo  = tipo de la lista  DSLS_S/DSLS_D
salida    : Ninguna
-------------------------------------------------------------------*/
void break_list_ls(hls_ptr lista, ls_ptr nodo,hls_ptr lis2,int tipo)
{
  dls_ptr aux= first_ls(lista),ant=NULL;

  if(aux==(dls_ptr) nodo)
    {
       memcpy(lis2,lista,sizeof(LSHEAD));
       memset(lista,'\0',sizeof(LSHEAD));
       return;
    }
  while(aux && (aux!=(dls_ptr)nodo))
    {
       ant = aux;
       aux=next_dls(aux);
    }
  if(NO_exist(aux))
     {
       memset(lis2,'\0',sizeof(LSHEAD));
       return;
     }
  last_ls(lis2)=last_ls(lista);
  first_ls(lis2)=aux;
  last_ls(lista)=ant;
  if (tipo==DSLS_D) prev_dls(aux)= NULL;
  next_dls(last_ls(lista))=NULL;
  return;
}

/*-------------------------------------------------------------------
funci¢n   : CHANGE_node_ls
objetivo  : Cambia la informaci¢n contenida en el nodo2 al nodo1,
         (s¢lo la informaci¢n, NO el encabezado)
par metros: nodo1 = direcci¢n del nodo destino
         nodo2 = direcci¢n del nodo fuente
         bytes = tama¤o del nodo en bytes
         head  = tama¤o de la cabecera del nodo
salida    : Direccion del nodo1 que se intercambio, NULL si no se
         realizo el intercambio.
-------------------------------------------------------------------*/
void  *CHANGE_node_ls(char *nodo1,char *nodo2,int bytes, int head)
{
  char *nodo3;

  nodo1+= head;
  nodo2+= head;
  bytes-= head;
  nodo3 = (char *)malloc(bytes);
  if(NO_exist(nodo3)) return NULL;
  memcpy(nodo3,nodo1,bytes);
  memcpy(nodo1,nodo2,bytes);
  memcpy(nodo2,nodo3,bytes);
  free(nodo3);
  return ((void *) nodo1);
}

/*-------------------------------------------------------------------
funci¢n   : COPY_list_ls
objetivo  : Obtiene una copia de la lista2 en la lista1;
par metros: list1 = direcci¢n del inicio de la lista destino
         list2 = direcci¢n de la lista fuente
         bytes = tama¤o del nodo en bytes
salida    : Ninguna
-------------------------------------------------------------------*/
void *COPY_list_ls(hls_ptr list1, hls_ptr list2, int bytes,int tipo)
{
  char  *aux1,*aux4;
  dls_ptr aux2,aux3=NULL;
  int cabecera=sizeof(LSHEAD);

  aux2 = first_ls(list2);
  while (exist(aux2))
    {
       aux1 = (char *) calloc(1,bytes);
       if (NO_exist(aux1))
     {
       purge_ls(list1);
       return NULL;
     }
       if (tipo==DSLS_D)
       {
         if (NO_exist(aux3)) push_dls(list1,(dls_ptr)aux1);
         else insert_after_dls(list1,aux3,(dls_ptr) aux1);
       }
       else
       {
         if (NO_exist(aux3)) push_sls(list1,(ls_ptr)aux1);
         else insert_after_sls(list1,(ls_ptr)aux3,(ls_ptr) aux1);
       }
       aux3 = (dls_ptr) aux1;
       aux1+= cabecera;
       aux4 = (char *) aux2;
       aux4+= cabecera;
       memcpy(aux1,aux4,bytes-cabecera);
       aux2= next_dls(aux2);
    }
  return(list1);
}

/*-------------------------------------------------------------------
funci¢n   : COPY_node_dls   (MACRO)
objetivo  : Copia el contenido del nodo2 en el nodo1.
         (s¢lo la informaci¢n, NO el encabezado)
par metros: nodo1 = direcci¢n del nodo destino
         nodo2 = direcci¢n del nodo fuente
         bytes = tama¤o del nodo en bytes
         head  = tama¤o de la cabecera del nodo
salida    : Ninguna
-------------------------------------------------------------------*/
void COPY_node_ls(char *nodo1, char *nodo2, int bytes,int head)
{
  nodo1+= head;
  nodo2+= head;
  memcpy(nodo1,nodo2,bytes-head);
}

/*--------------------------------------------------------------------
funci¢n   : create_ls.   (MACRO)
objetivo  : crea la Cabecera de una Lista (gen‚rica) doblemente Ligada
par metros: lista = direcci¢n del inicio de la lista.
salida    : direcci¢n de inicio de la cabecera
       NULL si no existe espacio disponible
--------------------------------------------------------------------*/
hls_ptr create_ls(hls_ptr lista)
{
     return ((hls_ptr) NODE_ls(lista, sizeof(LSHEAD)));
}

/*--------------------------------------------------------------------
funci¢n   : delete_dls.
objetivo  : borra un "nodo" de una "lista" doblemente ligada.
par metros: lista = cabecera de la lista.
         nodo  = nodo que se desea borrar.
salida    : Direcci¢n del nodo borrado de la lista,
         NULL si el nodo no se encuentra en la lista
--------------------------------------------------------------------*/
void *delete_dls(hls_ptr lista, dls_ptr nodo)
{
    dls_ptr temp;

    if (first_ls(lista) == nodo)
       return((void *)pop_dls(lista));
    if (last_ls(lista) == nodo)
       return((void *)delete_last_dls(lista));

    temp = first_ls(lista);
    while(temp && temp != nodo)
       temp = next_dls(temp);
    if (temp == NULL) return NULL;
    next_dls(prev_dls(nodo))= next_dls(nodo);
    prev_dls(next_dls(nodo))= prev_dls(nodo);
    next_dls(nodo) = NULL;
    prev_dls(nodo) = NULL;
    return ((void *)nodo);
}

/*--------------------------------------------------------------------
funci¢n   : delete_sls.
objetivo  : borra un "nodo" de una "lista" simple ligada.
par metros: lista = cabecera de la lista.
         nodo  = nodo que se desea borrar.
salida    : Direcci¢n del nodo borrado de la lista,
         NULL si el nodo no se encuentra en la lista
--------------------------------------------------------------------*/
void *delete_sls(hls_ptr lista, ls_ptr nodo)
{
    ls_ptr temp;

    if (first_ls(lista) == (dls_ptr) nodo)
       return((void *)pop_sls(lista));
    temp =(ls_ptr) first_ls(lista);
    while(temp && next_sls(temp) != nodo)
       temp = next_sls(temp);
    if (temp == NULL) return NULL;
    if (last_ls(lista)== (dls_ptr) nodo)
     last_ls(lista) = (dls_ptr) temp;
    next_sls(temp)= next_sls(nodo);
    next_sls(nodo) = NULL;
    return ((void *) nodo);
}

/*--------------------------------------------------------------------
funci¢n   : delete_last_dls.
objetivo  : extrae el £ltimo nodo de la una lista doblemente ligada.
par metros: lista = direcci¢n de inicio de la cabecera de la lista.
salida    : direcci¢n del nodo que se extrae.
--------------------------------------------------------------------*/
void *delete_last_dls(hls_ptr lista)
{
    dls_ptr temp;

    if (NO_exist(last_ls(lista)))
      return NULL;
    temp = last_ls(lista);
    if (first_ls(lista) == temp)
       {
     first_ls(lista) = NULL;
     last_ls(lista) = NULL;
     return (void *)temp;
       }
    last_ls(lista) = prev_dls(temp);
    next_dls(prev_dls(temp)) = NULL;
    prev_dls(temp) = NULL;
    return (void *)temp;
}

/*--------------------------------------------------------------------
funci¢n   : delete_last_sls.
objetivo  : extrae el £ltimo nodo de la una lista ligada simple.
par metros: lista = direcci¢n de inicio de la cabecera de la lista.
salida    : direcci¢n del nodo que se extrae.
--------------------------------------------------------------------*/
void *delete_last_sls(hls_ptr lista)
{
    dls_ptr temp;

    if (NO_exist(last_ls(lista)))
      return NULL;
    temp = last_ls(lista);
    if (first_ls(lista) == temp)
       {
     first_ls(lista) = NULL;
     last_ls(lista) = NULL;
     return (void *)temp;
       }
    temp = last_ls(lista);
    last_ls(lista) = first_ls(lista);
    while(temp != last_ls(lista))
      last_ls(lista) = next_sls(last_ls(lista));
    next_sls(last_ls(lista))= NULL;
    return (void *)temp;
}

/*--------------------------------------------------------------------
funci¢n   : destroy_ls.      (MACRO)
objetivo  : borra completamente el contenido de una lista y elimina
         su cabecera
par metros: lista = direcci¢n de inicio de la cabecera de la lista.
SALIDA    : Ninguna
--------------------------------------------------------------------*/
void destroy_ls(hls_ptr *lista)
{
    purge_ls(*lista);
    free(*lista);
    *lista = NULL;
}

/*--------------------------------------------------------------------
funci¢n   : destroy_all_ls
objetivo  : borra completamente la "lista". Destruye la informacion
         asociada con cada nodo, usando una funcion definida por el
         usuario.
par metros: lista   = direccion de la cabecera de la lista
         funcion = funcion que recibe un nodo y destruye la
                informacion ligada a el.
salida    : ninguna
-------------------------------------------------------------------*/
void destroy_all_ls(hls_ptr *lista,void (*funcion)())
{
  purge_all_ls(*lista, funcion);  
  free(*lista);
  *lista=NULL;
}

/*--------------------------------------------------------------------
funci¢n   : insert_dls.
objetivo  : inserta un nodo al final de una lista doblemente ligada.
par metros: lista = direcci¢n de la cabecera de la lista.
         nodo  = nodo que se desea insertar.
salida    : ninguna.
--------------------------------------------------------------------*/
void insert_dls(hls_ptr lista, dls_ptr nodo)
{
    if (NO_exist(first_ls(lista)))
       {
     first_ls(lista) = nodo;
     last_ls(lista) = nodo;
     prev_dls(nodo)   = NULL;
       }
    else
       {
     prev_dls(nodo) = last_ls(lista);
     last_ls(lista) = nodo;
     next_dls(prev_dls(nodo)) = nodo;
       }
    next_dls(nodo) = NULL;
}

/*--------------------------------------------------------------------
funci¢n   : insert_sls.
objetivo  : inserta un nodo al final de una lista ligada simple.
par metros: lista = direcci¢n de la cabecera de la lista.
         nodo  = nodo que se desea insertar.
salida    : ninguna.
--------------------------------------------------------------------*/
void insert_sls(hls_ptr lista, ls_ptr nodo)
{
    ls_ptr temp;

    if (NO_exist(first_ls(lista)))
       first_ls(lista) = (dls_ptr) nodo;
    else
       next_sls(last_ls(lista))=(dls_ptr)nodo;
    last_ls(lista) = (dls_ptr) nodo;
    next_sls(nodo) = NULL;
}

/*--------------------------------------------------------------------
funci¢n   : insert_after_dls.
objetivo  : inserta el "nodo2" despu‚s del "nodo1" en una lista
         doblemente ligada.
par metros: lista = cabecera de la lista.
         nodo1 = nodo de referencia.
         nodo2 = nodo que se desea insertar.
salida    : Ninguna
--------------------------------------------------------------------*/
void insert_after_dls(hls_ptr lista,dls_ptr nodo1,dls_ptr nodo2)
{
    prev_dls(nodo2) = nodo1;
    next_dls(nodo2) = next_dls(nodo1);
    next_dls(nodo1) = nodo2;
    if (last_ls(lista) == nodo1)
      last_ls(lista) = nodo2;
    else
      prev_dls(next_dls(nodo2)) = nodo2;
}

/*--------------------------------------------------------------------
funci¢n   : insert_after_sls.
objetivo  : inserta el "nodo2" despu‚s del "nodo1" en una lista
         ligada simple.
par metros: lista = cabecera de la lista.
         nodo1 = nodo de referencia.
         nodo2 = nodo que se desea insertar.
salida    : Ninguna
--------------------------------------------------------------------*/
void insert_after_sls(hls_ptr lista,ls_ptr nodo1,ls_ptr nodo2)
{
    next_sls(nodo2) = next_sls(nodo1);
    next_sls(nodo1) = nodo2;
    if (last_ls(lista) == (dls_ptr) nodo1)
     last_ls(lista) = (dls_ptr) nodo2;
}

/*--------------------------------------------------------------------
funci¢n   : insert_before_dls
objetivo  : inserta el "nodo2" antes del "nodo1" en una lista
         doblemente ligada.
par metros: lista = cabecera de la lista.
         nodo1 = nodo de referencia.
         nodo2 = nodo que se desea insertar.
salida    : Ninguna
--------------------------------------------------------------------*/
void insert_before_dls(hls_ptr lista, dls_ptr nodo1, dls_ptr nodo2)
{
    next_dls(nodo2) = nodo1;
    prev_dls(nodo2) = prev_dls(nodo1);
    prev_dls(nodo1) = nodo2;
    if (first_ls(lista) == nodo1)
       first_ls(lista) = nodo2;
    else
       next_dls(prev_dls(nodo2)) = nodo2;
}

/*--------------------------------------------------------------------
funci¢n   : insert_before_sls
objetivo  : inserta el "nodo2" antes del "nodo1" en una lista
         ligada simple.
par metros: lista = cabecera de la lista.
         nodo1 = nodo de referencia.
         nodo2 = nodo que se desea insertar.
salida    : Ninguna
--------------------------------------------------------------------*/
void insert_before_sls(hls_ptr lista, ls_ptr nodo1, ls_ptr nodo2)
{
   ls_ptr temp;

   if (first_ls(lista)== (dls_ptr)nodo1)
      first_ls(lista)=(dls_ptr)nodo2;
   else
     {
     temp= (ls_ptr) first_ls(lista);
     while (temp && next_sls(temp)!=nodo1)
           temp=next_sls(temp);
     if (NO_exist(temp)) return;
     next_sls(temp)=nodo2;
     }
   next_sls(nodo2) = nodo1;
}

/*--------------------------------------------------------------------
funci¢n   : insert_list_dls
objetivo  : inserta la "lista2" al final de la "lista1" y limpia la
         cabecera de la "lista2"
par metros: lista1 = direccion de la cabecera de la lista1
         lista2 = direccion de la cabecera de la lista2
salida    : ninguna
-------------------------------------------------------------------*/
void insert_list_dls(hls_ptr lista1,hls_ptr lista2)
{
   dls_ptr aux;

   if (empty_ls(lista1))
       memcpy(lista1,lista2,sizeof(LSHEAD));
   else
    {
     prev_dls(first_ls(lista2))=last_ls(lista1);
     next_dls(last_ls(lista1))=first_ls(lista2);
     last_ls(lista1)=last_ls(lista2);
     }
   memset(lista2,'\0',sizeof(LSHEAD));
}

/*--------------------------------------------------------------------
funci¢n   : insert_list_sls
objetivo  : inserta la "lista2" al final de la "lista1" y limpia la
         cabecera de la "lista2"
par metros: lista1 = direccion de la cabecera de la lista1
         lista2 = direccion de la cabecera de la lista2
salida    : ninguna
-------------------------------------------------------------------*/
void insert_list_sls(hls_ptr lista1,hls_ptr lista2)
{
   dls_ptr aux;

   if (empty_ls(lista1))
       memcpy(lista1,lista2,sizeof(LSHEAD));
   else
    {
     next_sls(last_ls(lista1))=first_ls(lista2);
     last_ls(lista1)=last_ls(lista2);
     }
   memset(lista2,'\0',sizeof(LSHEAD));
}

/*--------------------------------------------------------------------
funci¢n   : insert_list_after_dls
objetivo  : inserta la "lista2" despues del nodo indicado y limpia la
         cabecera de la lista2.
par metros: lista1= direccion de la cabecera de la lista de referencia
         nodo  = direccion del nodo despues del cual se insertara
              la lista.
         lista2= direccion de la cabecera de la lista a insertar
salida    : ninguna
-------------------------------------------------------------------*/
void insert_list_after_dls(hls_ptr lista1,dls_ptr nodo,hls_ptr lista2)
{
   if(nodo==last_ls(lista1))
     insert_list_dls(lista1,lista2);
   else
    {
       prev_dls(first_ls(lista2))= nodo;
       next_dls(last_ls(lista2)) = next_dls(nodo);
       prev_dls(next_dls(nodo)) = last_ls(lista2);
       next_dls(nodo) = first_ls(lista2);
       memset(lista2,'\0',sizeof(LSHEAD));
    }
}

/*--------------------------------------------------------------------
funci¢n   : insert_list_after_sls
objetivo  : inserta la "lista2" despues del nodo indicado y limpia la
         cabecera de la lista2.
par metros: lista1= direccion de la cabecera de la lista de referencia
         nodo  = direccion del nodo despues del cual se insertara
              la lista.
         lista2= direccion de la cabecera de la lista a insertar
salida    : ninguna
-------------------------------------------------------------------*/
void insert_list_after_sls(hls_ptr lista1,ls_ptr nodo,hls_ptr lista2)
{
   if(nodo==(ls_ptr)last_ls(lista1))
     insert_list_sls(lista1,lista2);
   else
    {
       next_sls(last_ls(lista2)) =(dls_ptr) next_sls(nodo);
       next_sls(nodo) = (ls_ptr) first_ls(lista2);
       memset(lista2,'\0',sizeof(LSHEAD));
    }
}

/*--------------------------------------------------------------------
funci¢n   : insert_list_before_dls
objetivo  : inserta la "lista" antes del nodo indicado y limpia la
         cabecera de la lista.
par metros: lista1= direccion de la cabecera de la lista de referencia
         nodo  = direccion del nodo despues del cual se insertara
              la lista.
         lista2= direccion de la cabecera de la lista a insertar
salida    : ninguna
-------------------------------------------------------------------*/
void insert_list_before_dls(hls_ptr lista1,dls_ptr nodo,hls_ptr lista2)
{
   if(nodo==first_ls(lista1))
     push_list_dls(lista1,lista2);
   else
    {
       prev_dls(first_ls(lista2))= prev_dls(nodo);
       next_dls(last_ls(lista2)) = nodo;
       next_dls(prev_dls(nodo)) = first_ls(lista2);
       prev_dls(nodo) = last_ls(lista2);
       memset(lista2,'\0',sizeof(LSHEAD));
    }
}

/*--------------------------------------------------------------------
funci¢n   : insert_list_before_sls
objetivo  : inserta la "lista" antes del nodo indicado y limpia la
         cabecera de la lista.
par metros: lista1= direccion de la cabecera de la lista de referencia
         nodo  = direccion del nodo despues del cual se insertara
              la lista.
         lista2= direccion de la cabecera de la lista a insertar
salida    : ninguna
-------------------------------------------------------------------*/
void insert_list_before_sls(hls_ptr lista1,ls_ptr nodo,hls_ptr lista2)
{
   ls_ptr temp;

   if(nodo==(ls_ptr)first_ls(lista1))
       push_list_sls(lista1,lista2);
   else
    {
       temp= (ls_ptr)first_ls(lista1);
       while (temp && next_sls(temp)!=nodo)
          temp=next_sls(temp);
       if(NO_exist(temp)) return;
       next_sls(temp)= (ls_ptr) first_ls(lista2);
       next_sls(last_ls(lista2)) = (dls_ptr)nodo;
       memset(lista2,'\0',sizeof(LSHEAD));
    }
}

/*--------------------------------------------------------------------
funci¢n   : invert_dls.
objetivo  : invierte el orden de la lista doblemente ligada.
par metros: lista = cabecera de la lista.
salida    : ninguna.
--------------------------------------------------------------------*/
void invert_dls(hls_ptr lista)
{
    dls_ptr temp,nodo;

    for(nodo=lista; nodo; nodo=prev_dls(nodo))
       {
        temp = next_dls(nodo);
        next_dls(nodo)=prev_dls(nodo);
        prev_dls(nodo)=temp;
       }
}

/*--------------------------------------------------------------------
funci¢n   : is_member_ls
objetivo  : comprueba si un nodo pertenece a una lista.
par metros: lista = direcci¢n de inicio de la cabecera de la lista.
salida    :  1 =>  si se encuentra en la lista.
          0 =>  si NO se encuentra en la Lista.
--------------------------------------------------------------------*/
int is_member_ls(hls_ptr lista, ls_ptr nodo)
{
   ls_ptr temp;

   for_ls(lista, temp, SLHEAD)
       if (temp == nodo) return 1;
   return 0;
}

/*--------------------------------------------------------------------
funci¢n   : looking_for_ls
objetivo  : busca el primer nodo de la lista cuyos datos, al ser
            comparada con la informaci¢n (inf), cumpla con la funci¢n
            de comparaci¢n.
par metros: lista   = cabecera de la lista
            funci¢n = funci¢n que prueba si un nodo cumple la
                      condici¢n de busqueda en la lista y regresa:
                                   0      falso
                                 <> 0     cierto
            inf     = informaci¢n a comparar.
salida    : direcci¢n del nodo, NULL si no se encuentra
-------------------------------------------------------------------*/
void *looking_for_ls(hls_ptr lista, int (*funcion)(), void *inf)
{
   ls_ptr aux;
   int conta;

	 for(aux= (ls_ptr) first_ls(lista);
     aux && !(*funcion)(aux,inf);aux=next_sls(aux));
   return aux;
}


/*--------------------------------------------------------------------
funci¢n   : nth_node_ls
objetivo  : busca el n-esimo nodo de lista.
par metros: lista = direcci¢n de la cabecera de la lista.
         n   = n£mero del nodo que se desea buscar.
salida    : direcci¢n del n-esimo nodo de "lista"
         NULL si no se encuentra
--------------------------------------------------------------------*/
void *nth_node_ls(hls_ptr lista, int n)
{
    int i=0;
    ls_ptr temp;

    for (i=0,temp=(ls_ptr)first_ls(lista);
           i<n&&temp;i++,temp=next_sls(temp));
    return ((void *)temp);
}

/*--------------------------------------------------------------------
funci¢n   : NODE_ls.
objetivo  : crea e Inicializa un Nodo del tama¤o del Tipo de datos que
         utiliza el Usuario.
         Si node es NULL crea e inicializa la instancia,
              de lo contrario s¢lo la inicializa.
par metros: nodo = apuntador al nodo del usuario.
         size = tama¤o del tipo de dato que utiliza el usuario.
salida    : la direcci¢n del nodo creado/inicializado.
--------------------------------------------------------------------*/
void *NODE_ls(char *node, int size)
{
   char *aux;
   int i;

   if (NO_exist(node))
     {
       node = (char *) calloc(1,size);
       return ((void *) node);
     }
   memset(node,'\0',size);
   return ((void *) node);
}

/*--------------------------------------------------------------------
funci¢n   : move_ls.                             (MACRO)
objetivo  : mueve el contenido de la lista2 a la lista1 y limpia la
         cabecera de la lista2.
par metros: lista1 = direcci¢n de la cabecera de la lista destino.
         lista2 = direcci¢n de la cabecera de la lista fuente.
salida    : Ninguna.
--------------------------------------------------------------------*/
void move_ls(hls_ptr lista1, hls_ptr lista2)
{
    memcpy(lista1,lista2, sizeof(LSHEAD));
    memset(lista2,'\0',sizeof(LSHEAD));
}

/*--------------------------------------------------------------------
funci¢n   : pop_dls.
objetivo  : extrae el primer nodo de la "lista" doblemente ligada.
par metros: lista = direcci¢n de la cabecera de la lista.
salida    : direcci¢n del nodo que se extrae, NULL si no existen nodos
--------------------------------------------------------------------*/
void *pop_dls(hls_ptr lista)
{
    dls_ptr temp;

    if (NO_exist(last_ls(lista))) return NULL;
    temp = first_ls(lista);
    if (temp == last_ls(lista))
      {
     prev_dls(lista) = NULL;
     next_dls(lista) = NULL;
     return ((void *)temp);
      }
    first_ls(lista) = next_dls(temp);
    prev_dls(next_dls(temp)) = NULL;
    next_dls(temp) = NULL;
    return ((void *)temp);
}

/*--------------------------------------------------------------------
funci¢n   : pop_sls.
objetivo  : extrae el primer nodo de la "lista" de liga simple.
par metros: lista = direcci¢n de la cabecera de la lista.
salida    : direcci¢n del nodo que se extrae, NULL si no existen nodos
--------------------------------------------------------------------*/
void *pop_sls(hls_ptr lista)
{
    ls_ptr temp;

    if (NO_exist(last_ls(lista))) return NULL;
    temp = (ls_ptr)first_ls(lista);
    if (temp ==(ls_ptr) last_ls(lista))
      {
     prev_dls(lista) = NULL;
     next_dls(lista) = NULL;
     return temp;
      }
    first_ls(lista) = (dls_ptr)next_sls(temp);
    next_sls(temp) = NULL;
    return ((void *) temp);
}

/*--------------------------------------------------------------------
funci¢n   : purge_ls.
objetivo  : borra el contenido de una lista, y limpia su cabecera
par metros: lista = direcci¢n de inicio de la cabecera de la lista.
SALIDA    : Ninguna
--------------------------------------------------------------------*/
void purge_ls(hls_ptr lista)
{
    ls_ptr temp,aux;

    temp = (ls_ptr) first_ls(lista);
    while(temp)                   /* for_ls  */
       {
     aux = temp;
     temp= next_sls(temp);
     free(aux);
       }
    memset(lista,'\0',sizeof(LSHEAD));
}

/*--------------------------------------------------------------------
funci¢n   : purge_all_ls
objetivo  : borra completamente la "lista". Destruye la informacion
         asociada con cada nodo, usando una funcion definida por el
         usuario.
par metros: lista   = direccion de la cabecera de la lista
         funcion = funcion que recibe un nodo y destruye la
                informacion ligada a el.
salida    : ninguna
-------------------------------------------------------------------*/
void purge_all_ls(hls_ptr lista,void (*funcion)())
{
   dls_ptr  temp, aux;

   temp = first_ls(lista);
   while(exist(temp))
     {
       aux = temp;
       temp= next_dls(temp);
       (*funcion)(aux);
       free(aux);
     }
   memset(lista,'\0',sizeof(LSHEAD));
}

/*--------------------------------------------------------------------
funci¢n   : push_dls.
objetivo  : Inserta un nodo al inicio de una lista doblemente ligada.
par metros: lista = direcci¢n de la cabecera de la lista.
         nodo  = nodo que se desea insertar.
salida    : Ninguna.
--------------------------------------------------------------------*/
void push_dls(hls_ptr lista, dls_ptr nodo)
{
    if (NO_exist(first_ls(lista)))
      {
      first_ls(lista) = nodo;
      last_ls(lista) = nodo;
      next_dls(nodo)  = NULL;
      }
    else
      {
      next_dls(nodo)  = first_ls(lista);
      first_ls(lista) = nodo;
      prev_dls(next_dls(nodo)) = nodo;
      }
    prev_dls(nodo) = NULL;
}

/*--------------------------------------------------------------------
funci¢n   : push_sls.
objetivo  : Inserta un nodo al inicio de una lista ligada simple.
par metros: lista = direcci¢n de la cabecera de la lista.
         nodo  = nodo que se desea insertar.
salida    : Ninguna.
--------------------------------------------------------------------*/
void push_sls(hls_ptr lista, ls_ptr nodo)
{
    if (NO_exist(first_ls(lista)))
      {
      first_ls(lista) = (dls_ptr)nodo;
      last_ls(lista) = (dls_ptr)nodo;
      next_sls(nodo)  = NULL;
      }
    else
      {
      next_sls(nodo)  = (ls_ptr)first_ls(lista);
      first_ls(lista) = (dls_ptr)nodo;
      }
}

/*--------------------------------------------------------------------
funci¢n   : push_list_dls
objetivo  : inserta la "lista2" al inicio de la "lista1" y limpia la
         cabecera de la "lista2".
par metros: lista1 = direccion de la cabecera de la lista1 destino
         lista2 = direccion de la cabecera de la lista2 fuente
salida    : ninguna
-------------------------------------------------------------------*/
void push_list_dls(hls_ptr lista1,hls_ptr lista2)
{
   dls_ptr aux;

   if (empty_ls(lista1))
       memcpy(lista1,lista2,sizeof(LSHEAD));
   else
    {
     prev_dls(first_ls(lista1))=last_ls(lista2);
     next_dls(last_ls(lista2))=first_ls(lista1);
     first_ls(lista1)=first_ls(lista2);
     }
   memset(lista2,'\0',sizeof(LSHEAD));
}

/*--------------------------------------------------------------------
funci¢n   : push_list_sls
objetivo  : inserta la "lista2" al inicio de la "lista1" y limpia la
         cabecera de la "lista2".
par metros: lista1 = direccion de la cabecera de la lista1 destino
         lista2 = direccion de la cabecera de la lista2 fuente
salida    : ninguna
-------------------------------------------------------------------*/
void push_list_sls(hls_ptr lista1,hls_ptr lista2)
{
   dls_ptr aux;

   if (empty_ls(lista1))
       memcpy(lista1,lista2,sizeof(LSHEAD));
   else
    {
     next_dls(last_ls(lista2))=first_ls(lista1);
     first_ls(lista1)=first_ls(lista2);
     }
   memset(lista2,'\0',sizeof(LSHEAD));
}

/*--------------------------------------------------------------------
funci¢n   : sort_insert_dls.
objetivo  : inserta un nodo en forma ordenada en una lista doblemente
         ligada.
par metros:  lista  = direcci¢n de la cabecera de la lista.
          nodo   = nodo que se desea insertar.
         funcion = funci¢n que compara recibe dos nodos y devuelve
                un entero:  > 0  si nodo1 > nodo2
                       0   si nodo1 = nodo2
                      < 0  si nodo1 < nodo2
salida    : Ninguna.
--------------------------------------------------------------------*/
void sort_insert_dls(hls_ptr lista, dls_ptr nodo, int (*funcion)())
{
    dls_ptr temp;

    temp = first_ls (lista);
    while (temp&&(*funcion)(nodo,temp)>0)
       temp = next_dls (temp);
    if (temp)
       {
     next_dls(nodo) = temp;
     prev_dls(nodo) = prev_dls(temp);
     prev_dls(temp) = nodo;
     if (first_ls(lista) == temp)
         first_ls(lista) = nodo;
     else
         next_dls(prev_dls(nodo)) = nodo;
       }
    else
       insert_dls(lista,nodo);
}

/*--------------------------------------------------------------------
funci¢n   : sort_insert_sls.
objetivo  : inserta un nodo en forma ordenada en una lista doblemente
         ligada.
par metros:  lista  = direcci¢n de la cabecera de la lista.
          nodo   = nodo que se desea insertar.
         funcion = funci¢n que compara recibe dos nodos y devuelve
                un entero:  > 0  si nodo1 > nodo2
                       0   si nodo1 = nodo2
                      < 0  si nodo1 < nodo2
salida    : Ninguna.
--------------------------------------------------------------------*/
void sort_insert_sls(hls_ptr lista, ls_ptr nodo, int (*funcion)())
{
    ls_ptr temp, ant=NULL;

    temp =(ls_ptr) first_ls(lista);
    while (temp&&(*funcion)(nodo,temp)>0)
       {
       ant  = temp;
       temp = next_sls(temp);
       }
    if (temp)
       {
      next_sls(nodo)= temp;
      if (NO_exist(ant))
         first_ls(lista) = (dls_ptr) nodo;
      else
         next_sls(ant) = nodo;
       }
    else
       insert_sls(lista,nodo);
}
