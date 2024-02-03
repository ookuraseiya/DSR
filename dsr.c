
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "dsr.h"

uint8_t id[IP_LENGTH];
uint8_t broadcast[IP_LENGTH];
uint8_t next_id[IP_LENGTH];
dsr_p pkt;

void set_id(uint8_t own_id[IP_LENGTH])
{
	int i;
	for (i = 0; i < IP_LENGTH; i++)
	{
		id[i] = own_id[i];
	}
}

void set_broadcast(uint8_t ba[IP_LENGTH])
{
	int i;
	for (i = 0; i < IP_LENGTH; i++)
	{
		broadcast[i] = ba[i];
	}
}

uint8_t *get_next()
{
	return &next_id[0];
}

uint8_t check_same_id(uint8_t *a, uint8_t *b)
{
	int i;
	for (i = 0; i < IP_LENGTH; i++)
	{
		if (a[i] != b[i])
		{
			return 0;
		}
	}
	return 1;
}

uint8_t find_id_in_ri(dsr_p *p)
{
	int i;
	for (i = 0; i < p->rilen; i++)
	{
		if (check_same_id(&id[0], p->ri + i * IP_LENGTH))
		{
			return 1;
		}
	}
	return 0;
}

void set_next_id(dsr_p *p)
{
	if (p->ptype == RREQ)
	{
		memcpy(next_id, broadcast, IP_LENGTH);
		return;
	}
	int i;
	for (i = 1; i < p->rilen; i++)
	{
		memcpy(next_id, p->ri + (i - 1) * IP_LENGTH, IP_LENGTH);
		if (check_same_id(id, p->ri + i * IP_LENGTH))
		{
			return;
		}
	}
	// memset(next_id, 0, IP_LENGTH);
}

void generate_initial_rreq(dsr_p *p, uint8_t dest[IP_LENGTH])
{
	p->ptype = RREQ;
	int i;
	for (i = 0; i < IP_LENGTH; i++)
	{
		p->src[i] = id[i];
		p->dest[i] = dest[i];
	}
	p->rilen = 1;
	p->ri = malloc(p->rilen * IP_LENGTH);
	memcpy(p->ri, &id[0], IP_LENGTH);
	p->plen = dsr_packet_size(p);
	set_next_id(p);
}

uint8_t rreq_processing(dsr_p *p)
{
	if (find_id_in_ri(p))
	{
		return 0;
	}
	int i;
	if (check_same_id(id, p->dest))
	{
		p->ptype = RREP;
		uint8_t tmp;
		for (i = 0; i < IP_LENGTH; i++)
		{
			tmp = p->src[i];
			p->src[i] = p->dest[i];
			p->dest[i] = tmp;
		}
	}

	uint8_t *pre_ri = p->ri;
	p->ri = (uint8_t *)malloc((p->rilen + 1) * IP_LENGTH);
	memcpy(p->ri, pre_ri, p->rilen * IP_LENGTH);

	for (i = 0; i < IP_LENGTH; i++)
	{
		p->ri[i + p->rilen * IP_LENGTH] = id[i];
	}
	p->rilen++;
	free(pre_ri);
	memcpy(next_id, broadcast, IP_LENGTH);

	p->plen = dsr_packet_size(p);
	set_next_id(p);
	return p->ptype;
}

uint8_t rrep_processing(dsr_p *p)
{
	if (check_same_id(id, p->dest))
	{
		printf("route established\n");
		return 0;
	}
	set_next_id(p);
	return RREQ;
}

uint8_t rerr_processing(dsr_p *p)
{
	return RERR;
}

uint8_t data_processing(dsr_p *p)
{
	return DATA;
}

uint8_t packet_processing(dsr_p *p)
{
	switch (p->ptype)
	{
	case RREQ:
	{
		return rreq_processing(p);
	}
	case RREP:
	{
		return rrep_processing(p);
	}
	case RERR:
	{
		return rerr_processing(p);
	}
	case DATA:
	{
		return data_processing(p);
	}
	default:
	{
		return 0;
	}
	}
}

uint8_t packet_processing_buf(uint8_t *buf)
{
	if (pkt.rilen != 0)
	{
		free(pkt.ri);
	}
	initialize_dsr_packet(&pkt);
	dsr_desireialize(buf, &pkt);
	return packet_processing(&pkt);
}