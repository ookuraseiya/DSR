#include "dsr.h"
#include "dsr_packet.h"
#include <stdio.h>

int main(int argc, char **argv) {
    dsr_p dsr;
    dsr.ptype = 1;
    dsr.src[0] = 10, dsr.src[1] = 0, dsr.src[2] = 0, dsr.src[3] = 1;
    dsr.dest[0] = 10, dsr.dest[1] = 0, dsr.dest[2] = 0, dsr.dest[3] = 5;

    uint8_t ri1[IP_LENGTH] = {10, 0, 0, 1};
    uint8_t ri2[IP_LENGTH * 2] = {10, 0, 0, 1, 10, 0, 0, 2};
    dsr.rilen = 2;
    dsr.ri = &ri2[0];
    dsr.plen = dsr_packet_size(&dsr);
    int len = dsr.plen;
    printf("packet size=%d\n", len);
    uint8_t buf[len];
    dsr_serialize(&buf[0], &dsr);
    printf("buf=[");
    for (int i = 0; i < len; i++) {
        printf("%d ", buf[i]);
    }
    printf("]\n");
    dsr_p p2;
    initialize_dsr_packet(&p2);
    print_dsr_p(&p2);
    dsr_desireialize(&buf[0], &p2);
    print_dsr_p(&p2);

    printf("\n");
    printf("\n");

    dsr_p p3;
    uint8_t f1[IP_LENGTH] = {10, 0, 0, 4};
    uint8_t f2[IP_LENGTH] = {10, 0, 0, 10};
    uint8_t f3[IP_LENGTH] = {10, 0, 0, 9};
    uint8_t f4[IP_LENGTH] = {10, 0, 0, 8};

    set_id(f1);
    generate_initial_rreq(&p3, f2);
    print_dsr_p(&p3);

    set_id(f3);
    packet_processing(&p3);
    print_dsr_p(&p3);

    set_id(f4);
    packet_processing(&p3);
    print_dsr_p(&p3);

    set_id(f3);
    packet_processing(&p3);
    print_dsr_p(&p3);

    set_id(f2);
    packet_processing(&p3);
    print_dsr_p(&p3);

    // next_id配列の値を出力
    uint8_t *n = get_next();
    printf("next_id=[");
    for (int i = 0; i < IP_LENGTH; i++) {
        printf("%d ", n[i]);
    }
    printf("]\n\n");

    // next_id配列の値を出力
    set_id(f3);
    packet_processing(&p3);
    n = get_next();
    printf("next_id=[");
    for (int i = 0; i < IP_LENGTH; i++) {
        printf("%d ", n[i]);
    }
    printf("]\n\n");

    int p3len = dsr_packet_size(&p3);
    printf("p3lenの値: %d\n", p3len);
    uint8_t buf2[p3len];
    dsr_serialize(buf2, &p3);
    printf("p3=[%d", buf2[0]);
    for (int i = 1; i < p3len; i++) {
        printf(",%d", buf2[i]);
    }
    printf("]\n\n");

    dsr_p p4;
    initialize_dsr_packet(&p4);
    dsr_desireialize(buf2, &p4);

    printf("最終的な構造体の各値:\n");
    print_dsr_p(&p4);
}