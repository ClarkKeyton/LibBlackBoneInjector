import ctypes
import os

def ConnectDLLLibBlackBone():
    library = ctypes.windll.LoadLibrary("{}".format(os.getcwd() + "\\libblackboneinjector-python.dll"))
    return library
def Main():
    process = input("Write A Process Name: ")
    path_to_dll = input("Write A Path To DLL: ")
    ConnectDLLLibBlackBone().AttachProcess(process)
    ConnectDLLLibBlackBone().LoadLibraryInject(process, path_to_dll)
if __name__ == "__main__":
    Main()