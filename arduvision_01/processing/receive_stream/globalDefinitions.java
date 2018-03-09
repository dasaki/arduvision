
final class G_DEF {
                public final static int BAUDRATE = 500000;

                public final static char  LF           = '\n';    // Linefeed in ASCII
                public final static char  CR           = '\r';    // Carriage return in ASCII

                public final static int   SCR_W        = 640;
                public final static int   SCR_H        = 480;
                public final static int   F_W          = 80;
                public final static int   F_H          = 60;
                public final static int   BPP          = 2;
                public final static int   FONT_SIZE    = 18;
                public final static int   FONT_BKG_SIZE  = (int)(FONT_SIZE*(float)1.5);
                public final static float DRAW_SCALE   = (float)SCR_W / (float)F_W;

                public final static int   MAX_ROW_LEN      = F_W*BPP; // pixels + LF character
                public final static int   MAX_ROW_BUFF_LEN = MAX_ROW_LEN+1; // pixels + LF character
                public final static int   ACK_BUF_LEN      = 255; // serial buffer to store "ACK"+LF
                public final static long  SERIAL_TIMEOUT   = 50; // milliseconds to wait for ACK
        }

enum requestStatus_t {
    IDLE, REQUESTED, TIMEOUT, ARRIVING, RECEIVED, PROCESSING  
};

enum request_t {
    NONE(0),
    TRACKDARK(1), 
    TRACKBRIG(2),
    STREAM8PPB(G_DEF.F_W/8),
    STREAM4PPB(G_DEF.F_W/4),
    STREAM2PPB(G_DEF.F_W/2),
    STREAM1PPB(G_DEF.F_W),
    STREAM0PPB(G_DEF.F_W*G_DEF.BPP);
    
    private int value;    

    private request_t(int value) {
      this.value = value;
    }
  
    public int getParam() {
      return value;
    }
};

