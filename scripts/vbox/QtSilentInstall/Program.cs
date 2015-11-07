using System;
using System.Diagnostics;
using Accessibility;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Threading;
using System.Windows.Forms;

namespace QtSilentInstall
{
	internal class Accessible
	{
		IAccessible mElement;

		int mID;

		Accessible(IAccessible a, int i)
		{
			mElement = a;
			mID = i;
		}

		[DllImport("oleacc.dll")]
		internal static extern int AccessibleObjectFromWindow(
			 IntPtr hwnd,
			 uint id,
			 ref Guid iid,
			 [In, Out, MarshalAs(UnmanagedType.IUnknown)] ref object ppvObject);

		[DllImport("oleacc.dll")]
		public static extern uint AccessibleChildren(
			IAccessible paccContainer,
			int iChildStart,
			int cChildren,
			[Out] object[] rgvarChildren,
			out int pcObtained);

		public static Accessible fromWindow(IntPtr hWnd)
		{
			const int OBJID_WINDOW = 0x00000000;
			Guid guid = new Guid("{618736E0-3C3D-11CF-810C-00AA00389B71}");
			object obj = null;
			AccessibleObjectFromWindow(hWnd, OBJID_WINDOW, ref guid, ref obj);
			IAccessible acc = obj as IAccessible;
			if (acc != null)
				return new Accessible(acc, OBJID_WINDOW);

			return null;
		}
		public bool hasChildren
		{
			get
			{
				if (mID != 0) return false;
				return mElement.accChildCount > 0;
			}
		}
		public ICollection<Accessible> Children
		{
			get
			{
				List<Accessible> list = new List<Accessible>();
				int count = mElement.accChildCount;
				object[] children = new object[count];
				int j;
				AccessibleChildren(mElement, 0, count, children, out j);
				foreach (object o in children)
				{
					Accessible a = ElementFromObject(o);
					if (a != null)
						list.Add(a);
				}
				return list;
			}
		}

		public Accessible ElementFromObject(object o)
		{

			if (o is int)
				return new Accessible(mElement, (int)o);

			IAccessible acc = o as IAccessible;
			if (acc != null)
				return new Accessible(acc, 0);

			return null;
		}

		public string Name
		{
			get
			{
				try
				{
					return mElement.get_accName(mID);
				}
				catch
				{
					return string.Empty;
				}
			}
		}
		public void DoAction()
		{
			mElement.accDoDefaultAction(mID);
		}

		public AccessibleRole Role
		{
			get
			{
				return (AccessibleRole)mElement.get_accRole(mID);
			}
		}

		public bool Invisible
		{
			get
			{
				return (State & (int)AccessibleStates.Invisible) != 0;
			}
		}

		public int State
		{
			get
			{
				return (int)mElement.get_accState(mID);
			}
		}

		public bool Checked
		{
			get
			{
				return (State & (int)AccessibleStates.Checked) != 0;
			}
		}

		public Accessible Find(Predicate<Accessible> match)
		{
			return Descend(this.Children, match);
		}

		private static Accessible Descend(ICollection<Accessible> c, Predicate<Accessible> match)
		{
			foreach (Accessible a in c)
			{
				if (match(a))
					return a;

				if (a.hasChildren)
				{
					Accessible r = Descend(a.Children, match);
					if (r != null)
						return r;
				}
			}
			return null;
		}
	}

	internal class Name
	{
		private static string mName;
		public static Predicate<Accessible> StartsWith(string name)
		{
			mName = name;
			return new Predicate<Accessible>(IsMatch);
		}
		static bool IsMatch(Accessible a)
		{
			if (a == null || a.Name == null)
				return false;
			if (a.Invisible == false)
				return a.Name.StartsWith(mName);
			return false;
		}
	}

	internal class LicenseRadio
	{
		public static Predicate<Accessible> Match
		{
			get
			{
				return new Predicate<Accessible>(IsMatch);
			}
		}
		static bool IsMatch(Accessible a)
		{
			if (a == null)
				return false;

			if (a.Name == null && a.Role == AccessibleRole.RadioButton && a.Checked == false)
				return true;

			return false;
		}
	}

	internal class LaunchCheckBox
	{
		public static Predicate<Accessible> Match
		{
			get
			{
				return new Predicate<Accessible>(IsMatch);
			}
		}
		static bool IsMatch(Accessible a)
		{
			if (a == null)
				return false;

			if (a.Name == "Launch Qt Creator" && a.Role == AccessibleRole.CheckButton && a.Checked == true)
				return true;

			return false;
		}
	}

	internal class Program
	{
		public delegate bool EnumWindowsProc(IntPtr hwnd, int lParam);

		[DllImport("user32.dll")]
		[return: MarshalAs(UnmanagedType.Bool)]
		static extern bool EnumWindows(EnumWindowsProc lpEnumFunc, IntPtr lParam);

		[DllImport("user32.dll", SetLastError = true)]
		static extern uint GetWindowThreadProcessId(IntPtr hWnd, out uint lpdwProcessId);

		static IntPtr gHWND;
		static bool FindWindow(IntPtr hwnd, int lParam)
		{
			uint lpdwProcessId;
			GetWindowThreadProcessId(hwnd, out lpdwProcessId);
			if (lpdwProcessId == lParam)
			{
				gHWND = hwnd;
				return false;
			}
			return true;
		}
		static EnumWindowsProc ptr;

		static Accessible WaitFor(Process p, Predicate<Accessible> match)
		{
			return WaitFor(p, match, 100);
		}

		static Accessible WaitFor(Process p, Predicate<Accessible> match, int pause)
		{
			Accessible a;
			do
			{
				Thread.Sleep(pause);
				ptr = new EnumWindowsProc(FindWindow);
				EnumWindows(ptr, new IntPtr(p.Id));
				a = Accessible.fromWindow(gHWND);
			} while (a == null);

			Accessible b;
			do
			{
				Thread.Sleep(pause);
				b = a.Find(match);
			} while (b == null);
			return b;
		}

		static int Main(string[] args)
		{
			if (args.Length != 1)
				return 1639;
			if (args[0] == null)
				return 160;

			Process p = Process.Start(args[0]);
			WaitFor(p, Name.StartsWith("Next")).DoAction();
			WaitFor(p, Name.StartsWith("Skip")).DoAction();
			WaitFor(p, Name.StartsWith("Next")).DoAction();
			WaitFor(p, Name.StartsWith("Next")).DoAction();
			WaitFor(p, Name.StartsWith("Next")).DoAction();
			WaitFor(p, LicenseRadio.Match).DoAction();
			WaitFor(p, Name.StartsWith("Next")).DoAction();
			WaitFor(p, Name.StartsWith("Next")).DoAction();
			WaitFor(p, Name.StartsWith("Install")).DoAction();
			WaitFor(p, Name.StartsWith("Finish"), 5000);
			WaitFor(p, LaunchCheckBox.Match).DoAction();
			WaitFor(p, Name.StartsWith("Finish")).DoAction();
			p.WaitForExit();
			Console.WriteLine("Success!");
			return p.ExitCode;
		}
	}
}
