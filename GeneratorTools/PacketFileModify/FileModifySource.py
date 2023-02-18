
class FileModifySource():
	def __init__(self):
		self.OldStr = []
		self.NewStr = []

	def file_modify(self, path):
		f = open(path, mode='r+', encoding='utf8')
		lines = f.readlines()

		self.NewStr = {
				"#if defined(_MSC_VER)\n"
				"        #pragma warning (disable:4018) // 'expression' : signed/unsigned mismatch\n"
				"        #pragma warning (disable:4065) // switch statement contains 'default' but no 'case' labels\n"
				"        #pragma warning (disable:4146) // unary minus operator applied to unsigned type, result still unsigned\n"
				"        #pragma warning (disable:4244) // 'conversion' conversion from 'type1' to 'type2', possible loss of data\n"
				"        #pragma warning (disable:4251) // 'identifier' : class 'type' needs to have dll-interface to be used byclient of class 'type2'\n"
				"        #pragma warning (disable:4267) // 'var' : conversion from 'size_t' to 'type', possible loss of data\n"
				"        #pragma warning (disable:4305) // 'identifier' : truncation from 'type1' to 'type2'\n"
				"        #pragma warning (disable:4307) // 'operator' : integral constant overflow\n"
				"        #pragma warning (disable:4309) // 'conversion' : truncation of constant value\n"
				"        #pragma warning (disable:4334) // 'operator' : result of 32-bit shift implicitly converted to 64 bits (was64bit shift intended?)\n"
				"        #pragma warning (disable:4355) // 'this' : used in base member initializer list\n"
				"        #pragma warning (disable:4506) // no definition for inline function 'function'\n"
				"        #pragma warning (disable:4800) // 'type' : forcing value to bool 'true' or 'false' (performance warning)\n"
				"		 #pragma warning (disable:4946) // reinterpret_cast used between related classes: 'class1' and 'class2'\n"
				"        #pragma warning (disable:4996) // The compiler encountered a deprecated declaration.\n"
				"        #pragma warning (disable:4125) // decimal digit terminates octal escape sequence\n"
				"#endif\n\n"
			}

		for line in lines:
			curRead = line
			self.OldStr.append(curRead)

		f.close()

		fw = open(path, mode='w', encoding='utf8')

		for string in self.NewStr:
			fw.write(string)

		for string in self.OldStr:
			fw.write(string)
		
		fw.close()