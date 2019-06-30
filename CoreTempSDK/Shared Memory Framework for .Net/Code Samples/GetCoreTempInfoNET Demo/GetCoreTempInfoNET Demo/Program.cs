using System;
using System.Collections.Generic;
using System.Text;
using System.Timers;
using GetCoreTempInfoNET;

namespace GetCoreTempInfoNET.Demo
{
    class Program
    {
        static Timer RefreshInfo;
        static CoreTempInfo CTInfo;

        static void Main(string[] args)
        {
            //Initiate CoreTempInfo class.
            CTInfo = new CoreTempInfo();
            //Sign up for an event reporting errors
            CTInfo.ReportError += new ErrorOccured(CTInfo_ReportError);

            //Initiate a timer.
            RefreshInfo = new Timer();
            RefreshInfo.Interval = 1000;
            RefreshInfo.Elapsed += new ElapsedEventHandler(RefreshInfo_Elapsed);

            //Get info manually.
            RefreshInfo_Elapsed(null, null);

            //Start timer counter.
            RefreshInfo.Start();

            //Press return to exit.
            Console.ReadLine();

            //Kill the timer/
            RefreshInfo.Stop();
            RefreshInfo.Dispose();
        }

        static void RefreshInfo_Elapsed(object sender, ElapsedEventArgs e)
        {
            //Clear the screen and post the title.
            Console.Clear();
            Console.WriteLine("Core Temp shared memory reader:\n");

            //Attempt to read shared memory.
            bool bReadSuccess = CTInfo.GetData();

            //If read was successful the post the new info on the console.
            if (bReadSuccess)
            {
                uint index;
                char TempType;
                if (CTInfo.IsFahrenheit)
                    TempType = 'F';
                else
                    TempType = 'C';
                Console.WriteLine("CPU Name: " + CTInfo.GetCPUName);
                Console.WriteLine("CPU Speed: " + CTInfo.GetCPUSpeed + "MHz (" + CTInfo.GetFSBSpeed + " x " + CTInfo.GetMultiplier + ")");
                Console.WriteLine("CPU VID: " + CTInfo.GetVID + "v");
                Console.WriteLine("Physical CPUs: " + CTInfo.GetCPUCount);
                Console.WriteLine("Cores per CPU: " + CTInfo.GetCoreCount);
                for (uint i = 0; i < CTInfo.GetCPUCount; i++)
                {
                    Console.WriteLine("CPU #{0}", i);
                    Console.WriteLine("Tj.Max: " + CTInfo.GetTjMax[i] + "°" + TempType);
                    for (uint g = 0; g < CTInfo.GetCoreCount; g++)
                    {
                        index = g + (i * CTInfo.GetCoreCount);
                        if (CTInfo.IsDistanceToTjMax)
                            Console.WriteLine("Core #{0}: {1}°{2} to TjMax, {3}% Load", index, CTInfo.GetTemp[index], TempType, CTInfo.GetCoreLoad[index]);
                        else
                            Console.WriteLine("Core #{0}: {1}°{2}, {3}% Load", index, CTInfo.GetTemp[index], TempType, CTInfo.GetCoreLoad[index]);
                    }
                }
            }
            else
            {
                Console.WriteLine();
                Console.WriteLine("Internal error name: " + CTInfo.GetLastError);
                Console.WriteLine("Internal error value: " + (int)CTInfo.GetLastError);
                Console.WriteLine("Internal error message: " + CTInfo.GetErrorMessage(CTInfo.GetLastError));
            }
        }
        
        static void CTInfo_ReportError(ErrorCodes ErrCode, string ErrMsg)
        {
            Console.ForegroundColor = ConsoleColor.Red;
            Console.WriteLine(ErrMsg);
            Console.ResetColor();
        }
    }
}
