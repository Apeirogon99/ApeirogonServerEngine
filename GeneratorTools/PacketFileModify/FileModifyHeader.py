
class FileModifyHeader():
	def __init__(self):
		self.OldStr = []
		self.NewStr = []

	def file_modify(self, path):
		f = open(path, mode='r+', encoding='utf8')
		lines = f.readlines()

		self.NewStr = {
				"#if defined(_MSC_VER)\n"
				"		 #pragma warning (disable:4946) // reinterpret_cast used between related classes: 'class1' and 'class2'\n"
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