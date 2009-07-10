//----------------------------------------------------------------------------
// NAME: Multitouch control
// VERSION: 1.0
// DESCRIPTION: Remotecontrol with multitouch table
// DEVELOPER: Laurence Muller (aka Falcon4ever)
// E-MAIL: info@falcon4ever.com
// LICENSE: GNU General Public License v2
// WEBSITE: http://www.multigesture.net/
//----------------------------------------------------------------------------

/* 
 * References:
 * http://www.worldwindcentral.com/wiki/Plugin_FAQ
 * http://www.worldwindcentral.com/wiki/Compiling_the_sources
*/

using System.Windows.Forms;
using WorldWind.Renderable;
using WorldWind.Camera;
using WorldWind;
using System.IO;
using System;
using System.Threading;
using System.Reflection;
using System.Diagnostics;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using TouchlibCOMwrapper;

namespace LMuller.Plugin
{
    class Handler : COM_ITouchListener
    {
        public void fingerDown(TouchData data)
        {
            Multitouch.fingerList[data.ID] = data;            
            //Debug.Print("fingerDown C# nasa\n");
        }

        public void fingerUpdate(TouchData data)
        {
            Multitouch.fingerList[data.ID] = data;
            //Debug.Print("fingerUpdate C# nasa\n");
        }

        public void fingerUp(TouchData data)
        {
            Multitouch.fingerList.Remove(data.ID);
            //Debug.Print("fingerUp C# nasa\n");
        }
    }

    /// <summary>
    /// The plugin (main class)
    /// </summary>
    public class Multitouch : WorldWind.PluginEngine.Plugin
    {
        // Plugin
        DrawArgs drawArgs;
        Thread multitouchThread;

        // Misc
        const double RotationFactor = 5e-4f;
        const double ZoomFactor = 1.2;
        const int AxisRange = 100;
        const double GRAD_PI = 180.0f / 3.14159f;

        // Multitouch
        int blob1;
        int blob2;        
        int state;

        double prev_x1;
        double prev_y1;
        double prev_x2;
        double prev_y2;
        double previous_distance;
        
        // Wrapper stuff
        bool running = true;
        COM_ITouchListener listener;
        public static SortedList<int, TouchData> fingerList = new SortedList<int, TouchData>();

        /// <summary>
        /// Plugin entry point 
        /// </summary>
        public override void Load()
        {
            drawArgs = ParentApplication.WorldWindow.DrawArgs;

            Debug.Print("Loading Multitouch Plugin!\n");
            multitouchThread = new Thread(new ThreadStart(MultitouchLoop));
            multitouchThread.IsBackground = true;
            multitouchThread.Start();
        }

        /// <summary>
        /// Unloads our plugin
        /// </summary>
        public override void Unload()
        {
            //running = false;
            Debug.Print("Unloading Multitouch Plugin!\n");
            if (multitouchThread != null && multitouchThread.IsAlive)
                multitouchThread.Abort();
            multitouchThread = null;
        }
        
        /// <summary>
        /// Background thread runs this function in a loop reading multitouch state.
        /// </summary>
        void MultitouchLoop()
        {
            listener = new Handler(); // Create it

            try
            {
                init_touchlib(true, listener);
            }
            catch (DllNotFoundException e)
            {
                Console.WriteLine(e.ToString());    // Prob. couldnt locate the file TouchlibCOMwrapper.dll
                return;
            }

            do
            {
                getEvents();		// Update environment
                assignBlobs();
                processBlobs();

            } while (running);
        }

        void assignBlobs()
        {
            if (fingerList.Count == 0)  // State clear
            {
                state = 0;
                blob1 = 0;
                blob2 = 0;
                prev_x1 = 0;
                prev_y1 = 0;
                prev_x2 = 0;
                prev_y2 = 0;
                previous_distance = -1.0;
            }
            else if (fingerList.Count == 1) // State dragging
            {
                state = 1;
                foreach (KeyValuePair<int, TouchData> entry in fingerList)
                {
                    blob1 = entry.Value.ID;
                }
            }
            else if (fingerList.Count == 2) // State rotate/scale
            {
                state = 2;

                int x = 0;
                foreach (KeyValuePair<int, TouchData> entry in fingerList)
                {
                    if (x == 0)
                        blob1 = entry.Value.ID;
                    else
                        blob2 = entry.Value.ID;

                    x++;
                    if (x == 2)
                        break;
                }
            }
            else // If more points are found, default to rotate/scale
            {
                state = 2;

                int x = 0;
                foreach (KeyValuePair<int, TouchData> entry in fingerList)
                {
                    if (x == 0)
                        blob1 = entry.Value.ID;
                    else
                        blob2 = entry.Value.ID;

                    x++;
                    if (x == 2)
                        break;
                }
            }
        }

        void processBlobs()
        {
            switch (state)
            {
                case 0:
                    {
                        // Nothing!
                    }
                    break;

                case 1:
                    {
                        // Movement Up/Down/Left/Right                        
                        double scaling = 0.2 * RotationFactor * drawArgs.WorldCamera.ViewRange.Radians;
                        drawArgs.WorldCamera.RotationYawPitchRoll(
                            Angle.FromRadians(((-fingerList[blob1].dX * 3000)) * scaling),
                            Angle.FromRadians(((fingerList[blob1].dY * 3000)) * scaling),
                            Angle.Zero);
                    }
                    break;

                case 2:
                    {
                        bool first = false;                        

                        // Zoom in/out                        
                        double delta_x = (double)fingerList[blob1].X - (double)fingerList[blob2].X;
                        double delta_y = (double)fingerList[blob1].Y - (double)fingerList[blob2].Y;
                        double distance = Math.Sqrt(delta_x * delta_x + delta_y * delta_y);

                        if (previous_distance == -1.0)
                        {
                            previous_distance = distance;
                            first = true;
                        }

                        // Apply zooming
                        double delta_distance = previous_distance - distance;                        
                        double altitudeDelta = ZoomFactor * drawArgs.WorldCamera.Altitude * ((delta_distance * 50));                                                
                        drawArgs.WorldCamera.Altitude += altitudeDelta;

                        // Store distance for next frame
                        previous_distance = distance;
                        
                        // Rotation
                        double origLine_x = prev_x1 - prev_x2;
                        double origLine_y = prev_y1 - prev_y2;
                        double ang1 = getAngleTrig(origLine_x, origLine_y);
                        double ang2;

                        if (first)
                        {
                            ang2 = ang1;
                        }
                        else
                        {
                            double curLine_x = (double)fingerList[blob1].X - (double)fingerList[blob2].X;
                            double curLine_y = (double)fingerList[blob1].Y - (double)fingerList[blob2].Y;
                            ang2 = getAngleTrig(curLine_x, curLine_y);
                        }

                        double delta_rotation = ang1 - ang2;
                                                
                        // Prevent peak values (300+)
                        if ((   delta_rotation > 1.0 && delta_rotation < 25.0) || 
                            (   delta_rotation < -1.0  && delta_rotation > -25.0))
                        {                            
                                // rotate plz                                
                                drawArgs.WorldCamera.RotationYawPitchRoll(
                                Angle.Zero,
                                Angle.Zero,
                                Angle.FromRadians((delta_rotation * 20) * RotationFactor));
                        }

                        // Store distance for next frame
                        prev_x1 = (double)fingerList[blob1].X;
                        prev_y1 = (double)fingerList[blob1].Y;
                        prev_x2 = (double)fingerList[blob2].X;
                        prev_y2 = (double)fingerList[blob2].Y;
                    }
                    break;

                default:
                    break;
            }
        }

        double getAngleTrig(double X, double Y)
        {
	        if (X == 0.0)
	        {
		        if(Y < 0.0)
			        return 270;
		        else
			        return 90;
	        } else if (Y == 0)
	        {
		        if(X < 0)
			        return 180;
		        else
			        return 0;
	        }

	        if ( Y > 0.0)
		        if (X > 0.0)
			        return Math.Atan(Y/X) * GRAD_PI;
		        else
                    return (double)180.0 - Math.Atan(Y / -X) * GRAD_PI;
	        else
		        if (X > 0.0)
                    return (double)360.0 - Math.Atan(-Y / X) * GRAD_PI;
		        else
                    return (double)180.0 + Math.Atan(-Y / -X) * GRAD_PI;
        } 

        // Get the functions out of our wrapper dll
        [DllImport("TouchlibCOMwrapper.dll")]
        static extern void init_touchlib(bool debug, COM_ITouchListener listener);
        [DllImport("TouchlibCOMwrapper.dll")]
        static extern void kill_touchlib();
        [DllImport("TouchlibCOMwrapper.dll")]
        static extern void getEvents();
        [DllImport("TouchlibCOMwrapper.dll")]
        static extern void recapture_background();
        [DllImport("TouchlibCOMwrapper.dll")]
        static extern void auto_rectify();
    }
}
