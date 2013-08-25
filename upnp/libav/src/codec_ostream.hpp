/*
 * Copyright (C) 2013 midnightBITS
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __CODEC_OSTREAM_HPP__
#define __CODEC_OSTREAM_HPP__

namespace std
{
	inline ostream& operator << (ostream& o, AVCodecID codec)
	{
#define ENUM_STR(x) case AV_CODEC_ID_## x : return o << #x
#define ENUM_STR_4CC(x) case AV_CODEC_ID_## x : return o.write((char*)&codec, sizeof(codec))
		switch (codec)
		{
			ENUM_STR(NONE);
			ENUM_STR(MPEG1VIDEO);
			ENUM_STR(MPEG2VIDEO); ///< preferred ID for MPEG-1/2 video decoding
			ENUM_STR(MPEG2VIDEO_XVMC);
			ENUM_STR(H261);
			ENUM_STR(H263);
			ENUM_STR(RV10);
			ENUM_STR(RV20);
			ENUM_STR(MJPEG);
			ENUM_STR(MJPEGB);
			ENUM_STR(LJPEG);
			ENUM_STR(SP5X);
			ENUM_STR(JPEGLS);
			ENUM_STR(MPEG4);
			ENUM_STR(RAWVIDEO);
			ENUM_STR(MSMPEG4V1);
			ENUM_STR(MSMPEG4V2);
			ENUM_STR(MSMPEG4V3);
			ENUM_STR(WMV1);
			ENUM_STR(WMV2);
			ENUM_STR(H263P);
			ENUM_STR(H263I);
			ENUM_STR(FLV1);
			ENUM_STR(SVQ1);
			ENUM_STR(SVQ3);
			ENUM_STR(DVVIDEO);
			ENUM_STR(HUFFYUV);
			ENUM_STR(CYUV);
			ENUM_STR(H264);
			ENUM_STR(INDEO3);
			ENUM_STR(VP3);
			ENUM_STR(THEORA);
			ENUM_STR(ASV1);
			ENUM_STR(ASV2);
			ENUM_STR(FFV1);
			ENUM_STR(4XM);
			ENUM_STR(VCR1);
			ENUM_STR(CLJR);
			ENUM_STR(MDEC);
			ENUM_STR(ROQ);
			ENUM_STR(INTERPLAY_VIDEO);
			ENUM_STR(XAN_WC3);
			ENUM_STR(XAN_WC4);
			ENUM_STR(RPZA);
			ENUM_STR(CINEPAK);
			ENUM_STR(WS_VQA);
			ENUM_STR(MSRLE);
			ENUM_STR(MSVIDEO1);
			ENUM_STR(IDCIN);
			ENUM_STR(8BPS);
			ENUM_STR(SMC);
			ENUM_STR(FLIC);
			ENUM_STR(TRUEMOTION1);
			ENUM_STR(VMDVIDEO);
			ENUM_STR(MSZH);
			ENUM_STR(ZLIB);
			ENUM_STR(QTRLE);
			ENUM_STR(TSCC);
			ENUM_STR(ULTI);
			ENUM_STR(QDRAW);
			ENUM_STR(VIXL);
			ENUM_STR(QPEG);
			ENUM_STR(PNG);
			ENUM_STR(PPM);
			ENUM_STR(PBM);
			ENUM_STR(PGM);
			ENUM_STR(PGMYUV);
			ENUM_STR(PAM);
			ENUM_STR(FFVHUFF);
			ENUM_STR(RV30);
			ENUM_STR(RV40);
			ENUM_STR(VC1);
			ENUM_STR(WMV3);
			ENUM_STR(LOCO);
			ENUM_STR(WNV1);
			ENUM_STR(AASC);
			ENUM_STR(INDEO2);
			ENUM_STR(FRAPS);
			ENUM_STR(TRUEMOTION2);
			ENUM_STR(BMP);
			ENUM_STR(CSCD);
			ENUM_STR(MMVIDEO);
			ENUM_STR(ZMBV);
			ENUM_STR(AVS);
			ENUM_STR(SMACKVIDEO);
			ENUM_STR(NUV);
			ENUM_STR(KMVC);
			ENUM_STR(FLASHSV);
			ENUM_STR(CAVS);
			ENUM_STR(JPEG2000);
			ENUM_STR(VMNC);
			ENUM_STR(VP5);
			ENUM_STR(VP6);
			ENUM_STR(VP6F);
			ENUM_STR(TARGA);
			ENUM_STR(DSICINVIDEO);
			ENUM_STR(TIERTEXSEQVIDEO);
			ENUM_STR(TIFF);
			ENUM_STR(GIF);
			ENUM_STR(DXA);
			ENUM_STR(DNXHD);
			ENUM_STR(THP);
			ENUM_STR(SGI);
			ENUM_STR(C93);
			ENUM_STR(BETHSOFTVID);
			ENUM_STR(PTX);
			ENUM_STR(TXD);
			ENUM_STR(VP6A);
			ENUM_STR(AMV);
			ENUM_STR(VB);
			ENUM_STR(PCX);
			ENUM_STR(SUNRAST);
			ENUM_STR(INDEO4);
			ENUM_STR(INDEO5);
			ENUM_STR(MIMIC);
			ENUM_STR(RL2);
			ENUM_STR(ESCAPE124);
			ENUM_STR(DIRAC);
			ENUM_STR(BFI);
			ENUM_STR(CMV);
			ENUM_STR(MOTIONPIXELS);
			ENUM_STR(TGV);
			ENUM_STR(TGQ);
			ENUM_STR(TQI);
			ENUM_STR(AURA);
			ENUM_STR(AURA2);
			ENUM_STR(V210X);
			ENUM_STR(TMV);
			ENUM_STR(V210);
			ENUM_STR(DPX);
			ENUM_STR(MAD);
			ENUM_STR(FRWU);
			ENUM_STR(FLASHSV2);
			ENUM_STR(CDGRAPHICS);
			ENUM_STR(R210);
			ENUM_STR(ANM);
			ENUM_STR(BINKVIDEO);
			ENUM_STR(IFF_ILBM);
			ENUM_STR(IFF_BYTERUN1);
			ENUM_STR(KGV1);
			ENUM_STR(YOP);
			ENUM_STR(VP8);
			ENUM_STR(PICTOR);
			ENUM_STR(ANSI);
			ENUM_STR(A64_MULTI);
			ENUM_STR(A64_MULTI5);
			ENUM_STR(R10K);
			ENUM_STR(MXPEG);
			ENUM_STR(LAGARITH);
			ENUM_STR(PRORES);
			ENUM_STR(JV);
			ENUM_STR(DFA);
			ENUM_STR(WMV3IMAGE);
			ENUM_STR(VC1IMAGE);
			ENUM_STR(UTVIDEO);
			ENUM_STR(BMV_VIDEO);
			ENUM_STR(VBLE);
			ENUM_STR(DXTORY);
			ENUM_STR(V410);
			ENUM_STR(XWD);
			ENUM_STR(CDXL);
			ENUM_STR(XBM);
			ENUM_STR(ZEROCODEC);
			ENUM_STR(MSS1);
			ENUM_STR(MSA1);
			ENUM_STR(TSCC2);
			ENUM_STR(MTS2);
			ENUM_STR(CLLC);
			ENUM_STR(MSS2);
			ENUM_STR(VP9);
			ENUM_STR(AIC);
			ENUM_STR(ESCAPE130_DEPRECATED);
			ENUM_STR(G2M_DEPRECATED);

			ENUM_STR_4CC(BRENDER_PIX);
			ENUM_STR_4CC(Y41P);
			ENUM_STR_4CC(ESCAPE130);
			ENUM_STR_4CC(EXR);
			ENUM_STR_4CC(AVRP);

			ENUM_STR_4CC(012V);
			ENUM_STR_4CC(G2M);
			ENUM_STR_4CC(AVUI);
			ENUM_STR_4CC(AYUV);
			ENUM_STR_4CC(TARGA_Y216);
			ENUM_STR_4CC(V308);
			ENUM_STR_4CC(V408);
			ENUM_STR_4CC(YUV4);
			ENUM_STR_4CC(SANM);
			ENUM_STR_4CC(PAF_VIDEO);
			ENUM_STR_4CC(AVRN);
			ENUM_STR_4CC(CPIA);
			ENUM_STR_4CC(XFACE);
			ENUM_STR_4CC(SGIRLE);
			ENUM_STR_4CC(MVC1);
			ENUM_STR_4CC(MVC2);
			ENUM_STR_4CC(SNOW);
			ENUM_STR_4CC(WEBP);
			ENUM_STR_4CC(SMVJPEG);

			/* various PCM "codecs" */
			ENUM_STR(PCM_S16LE);
			ENUM_STR(PCM_S16BE);
			ENUM_STR(PCM_U16LE);
			ENUM_STR(PCM_U16BE);
			ENUM_STR(PCM_S8);
			ENUM_STR(PCM_U8);
			ENUM_STR(PCM_MULAW);
			ENUM_STR(PCM_ALAW);
			ENUM_STR(PCM_S32LE);
			ENUM_STR(PCM_S32BE);
			ENUM_STR(PCM_U32LE);
			ENUM_STR(PCM_U32BE);
			ENUM_STR(PCM_S24LE);
			ENUM_STR(PCM_S24BE);
			ENUM_STR(PCM_U24LE);
			ENUM_STR(PCM_U24BE);
			ENUM_STR(PCM_S24DAUD);
			ENUM_STR(PCM_ZORK);
			ENUM_STR(PCM_S16LE_PLANAR);
			ENUM_STR(PCM_DVD);
			ENUM_STR(PCM_F32BE);
			ENUM_STR(PCM_F32LE);
			ENUM_STR(PCM_F64BE);
			ENUM_STR(PCM_F64LE);
			ENUM_STR(PCM_BLURAY);
			ENUM_STR(PCM_LXF);
			ENUM_STR(S302M);
			ENUM_STR(PCM_S8_PLANAR);
			ENUM_STR(PCM_S24LE_PLANAR);
			ENUM_STR(PCM_S32LE_PLANAR);
			ENUM_STR(PCM_S16BE_PLANAR);

			/* various ADPCM codecs */
			ENUM_STR(ADPCM_IMA_QT);
			ENUM_STR(ADPCM_IMA_WAV);
			ENUM_STR(ADPCM_IMA_DK3);
			ENUM_STR(ADPCM_IMA_DK4);
			ENUM_STR(ADPCM_IMA_WS);
			ENUM_STR(ADPCM_IMA_SMJPEG);
			ENUM_STR(ADPCM_MS);
			ENUM_STR(ADPCM_4XM);
			ENUM_STR(ADPCM_XA);
			ENUM_STR(ADPCM_ADX);
			ENUM_STR(ADPCM_EA);
			ENUM_STR(ADPCM_G726);
			ENUM_STR(ADPCM_CT);
			ENUM_STR(ADPCM_SWF);
			ENUM_STR(ADPCM_YAMAHA);
			ENUM_STR(ADPCM_SBPRO_4);
			ENUM_STR(ADPCM_SBPRO_3);
			ENUM_STR(ADPCM_SBPRO_2);
			ENUM_STR(ADPCM_THP);
			ENUM_STR(ADPCM_IMA_AMV);
			ENUM_STR(ADPCM_EA_R1);
			ENUM_STR(ADPCM_EA_R3);
			ENUM_STR(ADPCM_EA_R2);
			ENUM_STR(ADPCM_IMA_EA_SEAD);
			ENUM_STR(ADPCM_IMA_EA_EACS);
			ENUM_STR(ADPCM_EA_XAS);
			ENUM_STR(ADPCM_EA_MAXIS_XA);
			ENUM_STR(ADPCM_IMA_ISS);
			ENUM_STR(ADPCM_G722);
			ENUM_STR(ADPCM_IMA_APC);
			ENUM_STR_4CC(VIMA);
			ENUM_STR_4CC(ADPCM_AFC);
			ENUM_STR_4CC(ADPCM_IMA_OKI);
			ENUM_STR_4CC(ADPCM_DTK);
			ENUM_STR_4CC(ADPCM_IMA_RAD);

			/* AMR */
			ENUM_STR(AMR_NB);
			ENUM_STR(AMR_WB);

			/* RealAudio codecs*/
			ENUM_STR(RA_144);
			ENUM_STR(RA_288);

			/* various DPCM codecs */
			ENUM_STR(ROQ_DPCM);
			ENUM_STR(INTERPLAY_DPCM);
			ENUM_STR(XAN_DPCM);
			ENUM_STR(SOL_DPCM);

			/* audio codecs */
			ENUM_STR(MP2);
			ENUM_STR(MP3); ///< preferred ID for decoding MPEG audio layer 1, 2 or 3
			ENUM_STR(AAC);
			ENUM_STR(AC3);
			ENUM_STR(DTS);
			ENUM_STR(VORBIS);
			ENUM_STR(DVAUDIO);
			ENUM_STR(WMAV1);
			ENUM_STR(WMAV2);
			ENUM_STR(MACE3);
			ENUM_STR(MACE6);
			ENUM_STR(VMDAUDIO);
			ENUM_STR(FLAC);
			ENUM_STR(MP3ADU);
			ENUM_STR(MP3ON4);
			ENUM_STR(SHORTEN);
			ENUM_STR(ALAC);
			ENUM_STR(WESTWOOD_SND1);
			ENUM_STR(GSM); ///< as in Berlin toast format
			ENUM_STR(QDM2);
			ENUM_STR(COOK);
			ENUM_STR(TRUESPEECH);
			ENUM_STR(TTA);
			ENUM_STR(SMACKAUDIO);
			ENUM_STR(QCELP);
			ENUM_STR(WAVPACK);
			ENUM_STR(DSICINAUDIO);
			ENUM_STR(IMC);
			ENUM_STR(MUSEPACK7);
			ENUM_STR(MLP);
			ENUM_STR(GSM_MS); /* as found in WAV */
			ENUM_STR(ATRAC3);
#if FF_API_VOXWARE
			ENUM_STR(VOXWARE);
#endif
			ENUM_STR(APE);
			ENUM_STR(NELLYMOSER);
			ENUM_STR(MUSEPACK8);
			ENUM_STR(SPEEX);
			ENUM_STR(WMAVOICE);
			ENUM_STR(WMAPRO);
			ENUM_STR(WMALOSSLESS);
			ENUM_STR(ATRAC3P);
			ENUM_STR(EAC3);
			ENUM_STR(SIPR);
			ENUM_STR(MP1);
			ENUM_STR(TWINVQ);
			ENUM_STR(TRUEHD);
			ENUM_STR(MP4ALS);
			ENUM_STR(ATRAC1);
			ENUM_STR(BINKAUDIO_RDFT);
			ENUM_STR(BINKAUDIO_DCT);
			ENUM_STR(AAC_LATM);
			ENUM_STR(QDMC);
			ENUM_STR(CELT);
			ENUM_STR(G723_1);
			ENUM_STR(G729);
			ENUM_STR(8SVX_EXP);
			ENUM_STR(8SVX_FIB);
			ENUM_STR(BMV_AUDIO);
			ENUM_STR(RALF);
			ENUM_STR(IAC);
			ENUM_STR(ILBC);
			ENUM_STR(OPUS_DEPRECATED);
			ENUM_STR(COMFORT_NOISE);
			ENUM_STR(TAK_DEPRECATED);
			ENUM_STR(METASOUND);
			ENUM_STR_4CC(FFWAVESYNTH);
			ENUM_STR_4CC(SONIC);
			ENUM_STR_4CC(SONIC_LS);
			ENUM_STR_4CC(PAF_AUDIO);
			ENUM_STR_4CC(OPUS);
			ENUM_STR_4CC(TAK);
			ENUM_STR_4CC(EVRC);
			ENUM_STR_4CC(SMV);

			/* subtitle codecs */
			ENUM_STR(DVD_SUBTITLE);
			ENUM_STR(DVB_SUBTITLE);
			ENUM_STR(TEXT);  ///< raw UTF-8 text
			ENUM_STR(XSUB);
			ENUM_STR(SSA);
			ENUM_STR(MOV_TEXT);
			ENUM_STR(HDMV_PGS_SUBTITLE);
			ENUM_STR(DVB_TELETEXT);
			ENUM_STR(SRT);
			ENUM_STR_4CC(MICRODVD);
			ENUM_STR_4CC(EIA_608);
			ENUM_STR_4CC(JACOSUB);
			ENUM_STR_4CC(SAMI);
			ENUM_STR_4CC(REALTEXT);
			ENUM_STR_4CC(SUBVIEWER1);
			ENUM_STR_4CC(SUBVIEWER);
			ENUM_STR_4CC(SUBRIP);
			ENUM_STR_4CC(WEBVTT);
			ENUM_STR_4CC(MPL2);
			ENUM_STR_4CC(VPLAYER);
			ENUM_STR_4CC(PJS);
			ENUM_STR_4CC(ASS);  ///< ASS as defined in Matroska

			/* other specific kind of codecs (generally used for attachments) */
			ENUM_STR(TTF);
			ENUM_STR_4CC(BINTEXT);
			ENUM_STR_4CC(XBIN);
			ENUM_STR_4CC(IDF);
			ENUM_STR_4CC(OTF);
			ENUM_STR_4CC(SMPTE_KLV);
			ENUM_STR_4CC(DVD_NAV);


			ENUM_STR(PROBE); ///< codec_id is not known (like AV_CODEC_ID_NONE) but lavf should attempt to identify it

			ENUM_STR(MPEG2TS);
			ENUM_STR(MPEG4SYSTEMS);
			ENUM_STR(FFMETADATA);
			default: return o << "UNKNOWN-CODEC(" << (int) codec << ")";
		};
#undef ENUM_STR
#undef ENUM_STR_4CC

	}
}

#endif //__CODEC_OSTREAM_HPP__
