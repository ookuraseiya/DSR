#ifndef DSR_H
#define DSR_H

#include "dsr_packet.h"
#include "dsr_constants.h"

void set_id(uint8_t own_id[IP_LENGTH]);
void set_broadcast(uint8_t ba[IP_LENGTH]);
uint8_t *get_next();
void generate_initial_rreq(dsr_p *p, uint8_t dest[IP_LENGTH]);
uint8_t packet_processing(dsr_p *p);
uint8_t packet_processing_buf(uint8_t *buf);
#endif