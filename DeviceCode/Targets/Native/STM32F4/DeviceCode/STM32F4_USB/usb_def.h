////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  STM32 USB OTG Register Definitions
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define USB_OTG_NUM_FIFOS                8
#define USB_OTG_NUM_CHANNELS            16


typedef struct
{
  // global registers
  __IO UINT32 GOTGCTL;
  __IO UINT32 GOTGINT;
  __IO UINT32 GAHBCFG;
  __IO UINT32 GUSBCFG;
  __IO UINT32 GRSTCTL;
  __IO UINT32 GINTSTS;
  __IO UINT32 GINTMSK;
  __IO UINT32 GRXSTSR;
  __IO UINT32 GRXSTSP;
  __IO UINT32 GRXFSIZ;
  union {
    __IO UINT32 HNPTXFSIZ;
    __IO UINT32 DIEPTXF0;
  };
  __IO UINT32 HNPTXSTS;
       UINT32 Res1[2];
  __IO UINT32 GCCFG;
  __IO UINT32 CID;
       UINT32 Res2[48];
  union {
    __IO UINT32 HPTXFSIZ;
    __IO UINT32 DIEPTXF[USB_OTG_NUM_FIFOS];
  };
       UINT32 Res3[184];
  // host mode registers
  __IO UINT32 HCFG;
  __IO UINT32 HFIR;
  __IO UINT32 HFNUM;
       UINT32 Res4;
  __IO UINT32 HPTXSTS;
  __IO UINT32 HAINT;
  __IO UINT32 HAINTMSK;
       UINT32 Res5[9];
  __IO UINT32 HPRT;
       UINT32 Res6[47];
  struct {
    __IO UINT32 CHAR;
         UINT32 Res7;
    __IO UINT32 INT;
    __IO UINT32 INTMSK;
    __IO UINT32 TSIZ;
         UINT32 Res8[3];
  } HC[USB_OTG_NUM_CHANNELS];
       UINT32 Res9[64];
  // device mode registers
  __IO UINT32 DCFG;
  __IO UINT32 DCTL;
  __IO UINT32 DSTS;
       UINT32 Res10;
  __IO UINT32 DIEPMSK;
  __IO UINT32 DOEPMSK;
  __IO UINT32 DAINT;
  __IO UINT32 DAINTMSK;
       UINT32 Res11[2];
  __IO UINT32 DVBUSDIS;
  __IO UINT32 DVBUSPULSE;
       UINT32 Res12;
  __IO UINT32 DIEPEMPMSK;
       UINT32 Res13[50];
  struct {
    __IO UINT32 CTL;
         UINT32 Res14;
    __IO UINT32 INT;
         UINT32 Res15;
    __IO UINT32 TSIZ;
         UINT32 Res16;
    __IO UINT32 TXFSTS;
         UINT32 Res17;
  } DIEP[USB_OTG_NUM_CHANNELS];
  struct {
    __IO UINT32 CTL;
         UINT32 Res18;
    __IO UINT32 INT;
         UINT32 Res19;
    __IO UINT32 TSIZ;
         UINT32 Res20[3];
  } DOEP[USB_OTG_NUM_CHANNELS];
       UINT32 Res21[64];
  // power and clock gating
  __IO UINT32 PCGCCTL;
       UINT32 Res22[127];
  // FIFO regions
  __IO UINT32 DFIFO[USB_OTG_NUM_FIFOS][1024];
}
OTG_TypeDef;


#define OTG_FS_BASE           (0x50000000)
#define OTG_HS_BASE           (0x40040000)
#define OTG_FS                ((OTG_TypeDef *) OTG_FS_BASE)  
#define OTG_HS                ((OTG_TypeDef *) OTG_HS_BASE)  


#define OTG_GUSBCFG_PHYSEL    (1<<6)
#define OTG_GUSBCFG_PHYLPCS   (1<<15)
#define OTG_GUSBCFG_FDMOD     (1<<30)

#define OTG_GCCFG_PWRDWN      (1<<16)
#define OTG_GCCFG_VBUSASEN    (1<<18)
#define OTG_GCCFG_VBUSBSEN    (1<<19)
#define OTG_GCCFG_SOFOUTEN    (1<<20)
#define OTG_GCCFG_NOVBUSSENS  (1<<21)

#define OTG_GAHBCFG_GINTMSK   (1<<0)
#define OTG_GAHBCFG_TXFELVL   (1<<7)

#define OTG_GINTSTS_MMIS      (1<<1)
#define OTG_GINTSTS_OTGINT    (1<<2)
#define OTG_GINTSTS_RXFLVL    (1<<4)
#define OTG_GINTSTS_USBSUSP   (1<<11)
#define OTG_GINTSTS_USBRST    (1<<12)
#define OTG_GINTSTS_ENUMDNE   (1<<13)
#define OTG_GINTSTS_IEPINT    (1<<18)
#define OTG_GINTSTS_OEPINT    (1<<19)
#define OTG_GINTSTS_SRQINT    (1<<30)
#define OTG_GINTSTS_WKUPINT   (1U<<31)

#define OTG_GINTMSK_MMISM     (1<<1)
#define OTG_GINTMSK_OTGINT    (1<<2)
#define OTG_GINTMSK_RXFLVLM   (1<<4)
#define OTG_GINTMSK_USBSUSPM  (1<<11)
#define OTG_GINTMSK_USBRST    (1<<12)
#define OTG_GINTMSK_ENUMDNEM  (1<<13)
#define OTG_GINTMSK_IEPINT    (1<<18)
#define OTG_GINTMSK_OEPINT    (1<<19)
#define OTG_GINTMSK_SRQIM     (1<<30)
#define OTG_GINTMSK_WUIM      (1U<<31)

#define OTG_GRSTCTL_CSRST     (1<<0)
#define OTG_GRSTCTL_RXFFLSH   (1<<4)
#define OTG_GRSTCTL_TXFFLSH   (1<<5)
#define OTG_GRSTCTL_TXFNUM    (0x1F<<6)
#define OTG_GRSTCTL_TXF_ALL   (0x10<<6)

#define OTG_DCFG_DSPD         (0x3<<0)
#define OTG_DCFG_DAD          (0x7F<<4)

#define OTG_DCTL_RWUSIG       (1<<0)
#define OTG_DCTL_SDIS         (1<<1)
#define OTG_DCTL_POPRGDNE     (1<<11)

#define OTG_GRXSTSP_EPNUM     (0x0F<<0)
#define OTG_GRXSTSP_CHNUM     (0x0F<<0)
#define OTG_GRXSTSP_BCNT      (0x7FF<<4)
#define OTG_GRXSTSP_DPID      (0x03<<15)
#define OTG_GRXSTSP_PKTSTS    (0x0F<<17)
#define OTG_GRXSTSP_PKTSTS_GN (0x01<<17) // global OUT NAK
#define OTG_GRXSTSP_PKTSTS_PR (0x02<<17) // packet received
#define OTG_GRXSTSP_PKTSTS_DC (0x03<<17) // data transaction completed
#define OTG_GRXSTSP_PKTSTS_SC (0x04<<17) // setup stage completed
#define OTG_GRXSTSP_PKTSTS_TE (0x05<<17) // toggle error
#define OTG_GRXSTSP_PKTSTS_SR (0x06<<17) // setup data received
#define OTG_GRXSTSP_PKTSTS_CH (0x07<<17) // channel haltet
#define OTG_GRXSTSP_FRMNUM    (0x0F<<21)

#define OTG_DIEPMSK_XFRCM     (1<<0) // transfer completed
#define OTG_DIEPMSK_TOM       (1<<3) // timeout

#define OTG_DOEPMSK_XFRCM     (1<<0) // transfer completed
#define OTG_DOEPMSK_STUPM     (1<<3) // setup phase done

#define OTG_DIEPINT_XFRC      (1<<0) // transfer completed
#define OTG_DIEPINT_TOC       (1<<3) // timeout

#define OTG_DOEPINT_XFRC      (1<<0) // transfer completed
#define OTG_DOEPINT_STUP      (1<<3) // setup phase done

#define OTG_DIEPCTL_USBAEP    (1<<15)
#define OTG_DIEPCTL_STALL     (1<<21)
#define OTG_DIEPCTL_CNAK      (1<<26)
#define OTG_DIEPCTL_SNAK      (1<<27)
#define OTG_DIEPCTL_SD0PID    (1<<28)
#define OTG_DIEPCTL_EPDIS     (1<<30)
#define OTG_DIEPCTL_EPENA     (1U<<31)

#define OTG_DOEPCTL_USBAEP    (1<<15)
#define OTG_DOEPCTL_STALL     (1<<21)
#define OTG_DOEPCTL_CNAK      (1<<26)
#define OTG_DOEPCTL_SNAK      (1<<27)
#define OTG_DOEPCTL_SD0PID    (1<<28)
#define OTG_DOEPCTL_EPDIS     (1<<30)
#define OTG_DOEPCTL_EPENA     (1U<<31)

#define OTG_DIEPTSIZ_PKTCNT   (3<<19)
#define OTG_DIEPTSIZ_PKTCNT_1 (1<<19)

#define OTG_DOEPTSIZ_PKTCNT   (3<<19)
#define OTG_DOEPTSIZ_PKTCNT_1 (1<<19)
#define OTG_DOEPTSIZ_STUPCNT  (3<<29)
