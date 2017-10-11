/* -*- c++ -*- */
/*
 * Copyright 2017 Clayton Smith.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_NRSC5_L1_FM_ENCODER_IMPL_H
#define INCLUDED_NRSC5_L1_FM_ENCODER_IMPL_H

#include <nrsc5/l1_fm_encoder.h>

namespace gr {
  namespace nrsc5 {
    unsigned char V_PM[] = { 10, 2, 18, 6, 14, 8, 16, 0, 12, 4, 11, 3, 19, 7, 15, 9, 17, 1, 13, 5 };
    unsigned char V_PX2_MP5[] = { 0, 1, 2, 3 };
    unsigned char V_PX2_MP6[] = { 0, 1, 3, 2, 4, 5, 7, 6 };
    unsigned char V_SM[] = { 19, 7, 11, 3, 15, 1, 13, 9, 17, 5, 18, 6, 10, 2, 14, 0, 12, 8, 16, 4 };
    unsigned char V_SB[] = { 27, 7, 19, 12, 3, 23, 10, 1, 21, 9, 14, 25, 5, 17, 26, 6, 18, 13, 2, 22, 11, 0, 20, 8, 15, 24, 4, 16 };
    unsigned char V_SP[] = { 1, 0 };
    int REF_SC_CHAN[] = {
      478, 497, 516, 535, 554, 573, 592, 611, 630, 649, 668, 687, 706, 725, 744,
      745, 758, 777, 796, 815, 834, 853, 872, 891, 910, 929, 948, 967, 986, 1005, 1024, 1043, 1062, 1081, 1100, 1119, 1138, 1157, 1176, 1195, 1214, 1233, 1252, 1271, 1290, 1303,
      1304, 1323, 1342, 1361, 1380, 1399, 1418, 1437, 1456, 1475, 1494, 1513, 1532, 1551, 1570
    };
    int REF_SC_ID[] = {
      2, 1, 0, 3, 2, 1, 0, 3, 2, 1, 0, 3, 2, 1, 0,
      3, 2, 1, 0, 3, 2, 1, 0, 3, 2, 1, 0, 3, 2, 1, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
      0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2
    };

    class l1_fm_encoder_impl : public l1_fm_encoder
    {
     private:
      int psm;
      int p1_bits, p1_mod;
      int p2_bits, p2_mod;
      int p3_bits, p3_mod;
      int p4_bits, p4_mod;
      unsigned char buf[P1_BITS];
      unsigned char pids_g[PIDS_BITS * 5 / 2 * BLOCKS_PER_FRAME];
      unsigned char p1_g[P1_BITS * 5 / 2];
      unsigned char *p3_p4_g;
      int p1_prime_off;
      unsigned char *p1_prime;
      unsigned char *p1_prime_g;
      unsigned char pm_matrix[SYMBOLS_PER_FRAME * 20 * 36];
      unsigned char *px1_matrix;
      unsigned char *px2_matrix;
      unsigned char *px1_internal;
      unsigned char *px2_internal;
      int internal_half;
      unsigned char parity[128];
      unsigned char primary_sc_symbols[4][SYMBOLS_PER_FRAME];

      void reverse_bytes(const unsigned char *in, unsigned char *out, int len);
      void scramble(unsigned char *buf, int len);
      void conv_enc(int mode, const unsigned char *in, unsigned char *out, int len);
      void encode_l2_pdu(int mode, const unsigned char *in, unsigned char *out, int len);
      void interleaver_i(unsigned char *in, unsigned char *matrix, int J, int B, int C, int M, unsigned char *V, int N);
      void interleaver_ii(unsigned char *in, unsigned char *matrix, int J, int B, int C, int M, unsigned char *V, int b, int I0, int N);
      void interleaver_iii(unsigned char *in, unsigned char *matrix, int J, int B, int C, int M, unsigned char *V, int N);
      void interleaver_iv(unsigned char *matrix, unsigned char *internal, int half);
      void write_symbol(unsigned char *matrix_row, unsigned char *out_row, int *channels, int num_channels);
      void primary_sc_data_seq(unsigned char *out, int scid, int sci, int bc, int psmi);
      void secondary_sc_data_seq(unsigned char *out, int scid, int bc, int ssmi);
      int partitions_per_band();

     public:
      l1_fm_encoder_impl(const int psm);
      ~l1_fm_encoder_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace nrsc5
} // namespace gr

#endif /* INCLUDED_NRSC5_L1_FM_ENCODER_IMPL_H */
