#ifndef DSR_PACKET_H
#define DSR_PACKET_H

#include <stdint.h>
#include "dsr_constants.h"

typedef struct {
	uint8_t ptype;
	uint32_t plen;
	uint8_t src[IP_LENGTH];
	uint8_t dest[IP_LENGTH];
	uint8_t rilen;
	uint8_t *ri;
} dsr_p;

void initialize_dsr_packet(dsr_p *p);
int dsr_packet_size(const dsr_p* p);
void dsr_serialize(uint8_t *buf, const dsr_p *p);
void dsr_desireialize(const uint8_t *buf, dsr_p *p);
void print_dsr_p(dsr_p *p);
#endif