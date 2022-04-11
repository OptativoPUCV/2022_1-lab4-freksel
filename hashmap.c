#include "hashmap.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HashMap HashMap;
int enlarge_called = 0;

struct HashMap {
	Pair **buckets;
	long size;	 // cantidad de datos/pairs en la tabla
	long capacity; // capacidad de la tabla
	long current;  // indice del ultimo dato accedido
};

Pair *createPair(char *key, void *value) {
	Pair *new = (Pair *)malloc(sizeof(Pair));
	new->key = key;
	new->value = value;
	return new;
}

long hash(char *key, long capacity) {
	unsigned long hash = 0;
	char *ptr;
	for (ptr = key; *ptr != '\0'; ptr++) {
		hash += hash * 32 + tolower(*ptr);
	}
	return hash % capacity;
}

int is_equal(void *key1, void *key2) {
	if (key1 == NULL || key2 == NULL)
		return 0;
	if (strcmp((char *)key1, (char *)key2) == 0)
		return 1;
	return 0;
}

void insertMap(HashMap *map, char *key, void *value) {
  long index= hash(key,map->capacity);
  while(map->buckets[index]!=NULL && map->buckets[index]->key!=NULL){
    if(is_equal(map->buckets[index]->key,key)==1) return;
    index=(index+1) % map->capacity;
  }
  if(map->buckets[index]!=NULL){
    map->buckets[index]->key=key;
    map->buckets[index]->value=value;
    }
    else{
    map->buckets[index]= createPair(key,value);
    }
    map->size++;
  if((double)map->size/(double) map->capacity>0.75){
    enlarge(map);
    }
}

void enlarge(HashMap *map) {
  int Mo;
  int i=0;
  Pair** aux=map->buckets;
  Mo=map->capacity*2;
  HashMap *m2 = (HashMap*)malloc(sizeof(HashMap));
  m2->size=0;
  m2->capacity=Mo;
  m2->current=-1;
  
  while(i<m2->capacity/2){
    insertMap(m2,(m2->buckets[i]->key),aux);
    i++;
    hash(map->buckets[i]->key,m2->capacity);
    }
	enlarge_called = 1; // no borrar (testing purposes)
}

HashMap *createMap(long capacity) {
	HashMap *nueva = (HashMap *)malloc(sizeof(HashMap));

	nueva->size = 0;
	nueva->capacity = capacity;
	nueva->current = -1;

	nueva->buckets = (Pair **)calloc(capacity, sizeof(Pair **));

	return nueva;
}

void eraseMap(HashMap *map, char *key) {
  long index= hash(key,map->capacity);
   if(map->buckets[index]==NULL || map->buckets[index]->key==NULL){
     return;
     }
  while(map->buckets[index]!=NULL && (is_equal(map->buckets[index]->key,key)!=1)){
    index=(index+1) % map->capacity;
    }
    if(map->buckets[index]!=NULL){
    map->buckets[index]->key=NULL;
    map->size--;
    }
    return;
}

Pair *searchMap(HashMap *map, char *key) {
  long index2= hash(key,map->capacity);
  if(map->buckets[index2]==NULL || map->buckets[index2]->key==NULL){
    return NULL;
  }
  while(map->buckets[index2]!=NULL && (is_equal(map->buckets[index2]->key,key)!=1)){
    index2=(index2+1) % map->capacity;
    }
  map->current=index2;
  return map->buckets[index2];

}

Pair *firstMap(HashMap *map) {
  int i=0;
  while(map->buckets[i]==NULL || map->buckets[i]->key==NULL){
    i=(i+1) % map->capacity;
  }
  map->current=i;
	return map->buckets[i];
}

Pair *nextMap(HashMap *map) {
int i;
i=map->current;
i=(i+1)% map->capacity;
while(map->buckets[i]==NULL || map->buckets[i]->key==NULL){
  i=(i+1)% map->capacity;
}
if(i==0){
  return NULL;
}
map->current=i;
return map->buckets[i];
}