#include "dsr_packet.h"
#include <stdio.h>
#include <stdlib.h>

void initialize_dsr_packet(dsr_p *p) {
    p->ptype = 0;
    p->plen = 0;
    int i;
    for (i = 0; i < IP_LENGTH; i++) {
        p->src[i] = 0;
        p->dest[i] = 0;
    }
    p->rilen = 0;

    p->ri = 0;
}

int dsr_packet_size(const dsr_p *p) {
    return 1 + 4 + IP_LENGTH + IP_LENGTH + // ptype and src and dest
           1 +                             // size of rilen itself
           p->rilen * IP_LENGTH;           // length of ri
}

void dsr_serialize(uint8_t *buf, const dsr_p *p) {
    int i;
    buf[0] = p->ptype;
    buf++;
    for (i = 0; i < 4; i++) {
        *buf = (uint8_t)(p->plen >> (8 * i)) &
               0xFF; // 「*buf」で配列に値を格納できる。
        buf++;
    }
    for (i = 0; i < IP_LENGTH; i++) {
        buf[i] = p->src[i];
        buf[i + IP_LENGTH] = p->dest[i];
    }
    buf += IP_LENGTH * 2;
    *buf = p->rilen;
    buf++;
    for (i = 0; i < p->rilen * IP_LENGTH; i++) {
        buf[i] = p->ri[i];
    }
}

void dsr_desireialize(const uint8_t *buf, dsr_p *p) {
    int i;
    p->ptype = buf[0];
    buf++;
    p->plen = 0;

    for (i = 0; i < 4; i++) {
        p->plen += (((uint32_t)*buf) << (8 * i));
        buf++;
    }

    for (i = 0; i < IP_LENGTH; i++) {
        p->src[i] = buf[i];
        p->dest[i] = buf[i + IP_LENGTH];
    }
    buf += IP_LENGTH * 2;
    p->rilen = *buf;
    buf++;

    // 確保されたメモリがある場合、それを解放
    if (p->ri != 0) {
        free(p->ri);
    }

    // p->ri`が8バイト分のデータを保持させるためのメモリ作成
    p->ri = (uint8_t *)malloc(p->rilen * IP_LENGTH);

    for (i = 0; i < p->rilen * IP_LENGTH; i++) {
        p->ri[i] = buf[i];
    }
}

void print_dsr_p(dsr_p *p) {
    printf("type:%d,", p->ptype);
    printf("plen:%d,", p->plen);
    printf("src:[%d", p->src[0]);

    int i;
    for (i = 1; i < IP_LENGTH; i++) {
        printf(",%d", p->src[i]);
    }
    printf("], dest:[%d", p->dest[0]);

    for (i = 1; i < IP_LENGTH; i++) {
        printf(",%d", p->dest[i]);
    }
    printf("]\n");

    printf("rilen:%d", p->rilen);

    if (p->ri == 0) {
        printf(" []\n\n");
    } else {
        printf(" ri[%d", p->ri[0]);
        for (i = 1; i < p->rilen * IP_LENGTH; i++) {
            printf(",%d", p->ri[i]);
        }
        printf("]\n\n");
    }
}