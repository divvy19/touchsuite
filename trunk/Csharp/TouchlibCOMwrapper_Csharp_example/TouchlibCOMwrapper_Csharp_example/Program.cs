//////////////////////////////////////////////////////////////////////////////////////////
// Project description
// ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
// Name: Example Application
// Purpose: Example application of how to use touchlib with the wrapper in C#
//
// Author: Laurence Muller (aka Falcon4ever)
// Contact: info@falcon4ever.com
// Site: www.multigesture.net
//
// more info can be found TouchlibCOMwrapper dir (readme_COM.txt)
//////////////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Reflection;
using TouchlibCOMwrapper;

namespace TouchlibCOMwrapper_Csharp_example
{
    class Handler : COM_ITouchListener
    {
        public void fingerDown(TouchData data)
        {
            Program.fingerList[data.ID] = data;
            Console.WriteLine("fingerDown C#");            
        }

        public void fingerUpdate(TouchData data)
        {
            Program.fingerList[data.ID] = data;
            Console.WriteLine("fingerUpdate C#");            
        }

        public void fingerUp(TouchData data)
        {
            Program.fingerList.Remove(data.ID);    
            Console.WriteLine("fingerUp C#");                    
        }
    }

    class Program
    {
        static bool running = true;        
        public static SortedList<int, TouchData> fingerList = new SortedList<int, TouchData>();
        static COM_ITouchListener listener; // To hold the class

        static void Main(string[] args)
        {
            listener = new Handler(); // Create it

            try
            {
                init_touchlib(true, listener);
            }
            catch (DllNotFoundException e)
            {
                Console.WriteLine(e.ToString());    // Prob. couldnt locate the 
                return;
            }

            do
            {
                getEvents();		// Update environment

                // do smth here with the fingerList, for example:
                foreach (KeyValuePair<int, TouchData> entry in fingerList)
                {
                    Console.WriteLine(entry.Key + " => " + entry.Value.ID);
                }               

            } while (running);
        }

        // Get the functions out of our wrapper dll

        // Init Touchlib
        [DllImport("TouchlibCOMwrapper.dll")]
        static extern void init_touchlib(bool debug, COM_ITouchListener listener);

        // Exit touchlib
        [DllImport("TouchlibCOMwrapper.dll")]
        static extern void kill_touchlib();

        // Update pipeline 
        [DllImport("TouchlibCOMwrapper.dll")]
        static extern void getEvents();

        // Recapture the background (for subtraction)
        // When calling this function you should clear the fingerList manual
        [DllImport("TouchlibCOMwrapper.dll")]
        static extern void recapture_background();  

        // Auto rectify
        [DllImport("TouchlibCOMwrapper.dll")]
        static extern void auto_rectify();
    }
}