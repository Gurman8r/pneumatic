using System.Runtime.CompilerServices;

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public class Object
	{
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern void print(string message);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}