/** Implementation of the AST result structure.
 * @file result.c
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 1.0
 * @date 2021-07-31
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdlib.h>
#include <stdio.h>

#ifdef DEBUG
#include <assert.h>
#endif

#include "result.h"


/* ===== FUNCTIONS ===== */

int countResult(Result *res) {
	int c = 0;
	for (; res; res = res->next) {
		c++;
	}
	return c;
}

void printResult(Result *res) {
	switch (res->type) {
		case R_die:			printf("%dd{", res->integer); printValue(res->faces); printf("}"); break;
		case R_roll:		printValue(res->out); break;
		case R_set:			printf("{"); printValue(res->out);printf("}");  break;
		case R_int:			printf("%d", res->integer); break;
		case R_group: {
			for (; res; res = res->next) {
				printf("%d ", res->integer);
				switch (res->group) {
				case D_check:				printf("check "); break;
				case D_slashing:		printf("slashing "); break;
				case D_piercing:		printf("piercing "); break;
				case D_bludgeoning:	printf("bludgeoning "); break;
				case D_poison:			printf("poison "); break;
				case D_acid:				printf("acid "); break;
				case D_fire:				printf("fire "); break;
				case D_cold:				printf("cold "); break;
				case D_radiant:			printf("radiant "); break;
				case D_necrotic:		printf("necrotic "); break;
				case D_lightning:		printf("lightning "); break;
				case D_thunder:			printf("thunder "); break;
				case D_force:				printf("force "); break;
				case D_psychic:			printf("psychic "); break;
				case D_none:				break;
				}
			}
			// printf("%d", t->i);
			// switch (s->i) {
			// case D_check:				printf(" check"); break;
			// case D_slashing:		printf(" slashing"); break;
			// case D_piercing:		printf(" piercing"); break;
			// case D_bludgeoning:	printf(" bludgeoning"); break;
			// case D_poison:			printf(" poison"); break;
			// case D_acid:				printf(" acid"); break;
			// case D_fire:				printf(" fire"); break;
			// case D_cold:				printf(" cold"); break;
			// case D_radiant:			printf(" radiant"); break;
			// case D_necrotic:		printf(" necrotic"); break;
			// case D_lightning:		printf(" lightning"); break;
			// case D_thunder:			printf(" thunder"); break;
			// case D_force:				printf(" force"); break;
			// case D_psychic:			printf(" psychic"); break;
			// case D_none:				break;
			// }
		}
	}
}



/* ===== MEMORY MANAGEMENT ===== */

/**
 * Frees all pointers within @p res and sets their values to NULL.
 */
void freeResult(Result **res) {
	if (*res == NULL) return;
	
	switch ((*res)->type) {
	case R_roll:	freeValue(&(*res)->faces);
	case R_set:		freeValue(&(*res)->out); break;
	case R_die:		freeValue(&(*res)->faces); break;
	case R_group:	if ((*res)->next) freeResult(&(*res)->next); break;
	case R_int:
	}

	#ifdef DEBUG
	assert(!(*res)->faces && !(*res)->out  && !(*res)->next);
	#endif

	free(*res);
	*res = NULL;

	#ifdef DEBUG
	assert(!*res);
	#endif
}
