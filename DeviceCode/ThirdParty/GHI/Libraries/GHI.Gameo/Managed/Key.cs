using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
namespace GHI.Gameo
{
    public static class Key
    {
   

        const Cpu.Pin pinLeft = (Cpu.Pin)(1 * 16 + 13);//FEZCerberus.Pin.PB13; // BT7
        const Cpu.Pin pinRight = (Cpu.Pin)(1 * 16 + 15);//FEZCerberus.Pin.PB15; // BT9
        const Cpu.Pin pinUp = (Cpu.Pin)(2 * 16 + 5);//FEZCerberus.Pin.PC5; //BT6
        const Cpu.Pin pinPowerMenu = (Cpu.Pin)(0 * 16 + 0);//FEZCerberus.Pin.PA0; //BT1
        const Cpu.Pin pinDown = (Cpu.Pin)(1 * 16 + 14);//FEZCerberus.Pin.PB14; //BT8
        // Reserve 
        const Cpu.Pin reserve1 = (Cpu.Pin)(2 * 16 + 1);//FEZCerberus.Pin.PC1;    // BT2
        const Cpu.Pin reserve2 = (Cpu.Pin)(1 * 16 + 12);//FEZCerberus.Pin.PC2; // BT3
        const Cpu.Pin reserve3 = (Cpu.Pin)(2 * 16 + 3);//FEZCerberus.Pin.PC3; // BT4
        const Cpu.Pin reserve4 = (Cpu.Pin)(2 * 16 + 4);//FEZCerberus.Pin.PC4; // BT5

        static public Object Key_Left ;
        static public Object Key_Right;
        static public Object Key_Up;
        static public Object Key_Down;
        static public Object Key_PowerMenu;
        static public Object Key_C;
        static public Object Key_B;
        static public Object Key_A;
        static public Object Key_Start;
        static public void Initialize(Display.Orientation screenmode)
        {
            if (screenmode == Display.Orientation.Portrait)
            {
                Key_Left = new InputPort(pinLeft, false, Port.ResistorMode.PullDown);
                Key_Right = new InputPort(pinRight, false, Port.ResistorMode.PullDown);
                Key_Up = new InputPort(pinUp, false, Port.ResistorMode.PullDown);
                Key_Down = new InputPort(pinDown, false, Port.ResistorMode.PullDown);
                
            }
            else
            {
                Key_Left = new InputPort(pinUp, false, Port.ResistorMode.PullDown);
                Key_Right = new InputPort(pinDown, false, Port.ResistorMode.PullDown);
                Key_Up = new InputPort(pinRight, false, Port.ResistorMode.PullDown);
                Key_Down = new InputPort(pinLeft, false, Port.ResistorMode.PullDown);
                
            }
            Key_PowerMenu = new InputPort(pinPowerMenu, false, Port.ResistorMode.PullDown);
            Key_C = new InputPort(reserve1, false, Port.ResistorMode.PullDown);
            Key_B = new InputPort(reserve2, false, Port.ResistorMode.PullDown);
            Key_A = new InputPort(reserve3, false, Port.ResistorMode.PullDown);
            Key_Start = new InputPort(reserve4, false, Port.ResistorMode.PullDown);
        }
        static public void Dispose()
        {
            if (Key_Left != null) { ((InputPort)Key_Left).Dispose(); Key_Left = null; }
            if (Key_Right != null) { ((InputPort)Key_Right).Dispose(); Key_Right = null; }
            if (Key_Up != null) { ((InputPort)Key_Up).Dispose(); Key_Up = null; }
            if (Key_Down != null) { ((InputPort)Key_Down).Dispose(); Key_Down = null; }
            if (Key_PowerMenu != null) { ((InputPort)Key_PowerMenu).Dispose(); Key_PowerMenu = null; }
            if (Key_C != null) { ((InputPort)Key_C).Dispose(); Key_C = null; }
            if (Key_B != null) { ((InputPort)Key_B).Dispose(); Key_B = null; }
            if (Key_A != null) { ((InputPort)Key_A).Dispose(); Key_A = null; }
            if (Key_Start != null) { ((InputPort)Key_Start).Dispose(); Key_Start = null; }
        }
        static public Boolean IsKeyLeftPressed
        {
            get
            {
                if (Key_Left != null) return ((InputPort)Key_Left).Read();
                else return false;
            } 
        }
        static public Boolean IsKeyRightPressed
        {
            get
            {
                if (Key_Right != null) return ((InputPort)Key_Right).Read();
                else return false;
            } 
        }
        static public Boolean IsKeyUpPressed
        {
            get
            {
                if (Key_Up != null) return ((InputPort)Key_Up).Read();
                else return false;
            }
        }
        static public Boolean IsKeyDownPressed
        {
            get
            {
                if (Key_Down != null) return ((InputPort)Key_Down).Read();
                else return false;
            }
        }
        static public Boolean IsKeyPowerMenuPressed
        {
            get
            {
                if (Key_PowerMenu != null) return ((InputPort)Key_PowerMenu).Read();
                else return false;
            }
        }
        static public Boolean IsKeyCPressed
        {
            get
            {
                if (Key_C != null) return ((InputPort)Key_C).Read();
                else return false;
            }
        }
        static public Boolean IsKeyBPressed
        {
            get
            {
                if (Key_B != null) return ((InputPort)Key_B).Read();
                else return false;
            }
        }
        static public Boolean IsKeyAPressed
        {
            get
            {
                if (Key_A != null) return ((InputPort)Key_A).Read();
                else return false;
            } 
            
        }
        static public Boolean IsKeyStartPressed
        {
            get
            {
                if (Key_Start != null) return ((InputPort)Key_Start).Read();
                else return false;
            } 
            
        }
        
    }
}
