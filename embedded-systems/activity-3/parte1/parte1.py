from smbus import SMBus 
addr = 0x8 # addr do arduino 
bus = SMBus(1) 

print ("Digite 1 para Ler do potenciometero ou 0 para sair")
while flag:
    entrada = input(">>>> ") # solicita entrada no terminal
    if entrada == "1":
        (upper_byte, lower_byte) = bus.read_i2c_block_data(addr, 0, 2)
        total = upper_byte << 8 + lower_byte
        print("Potenciometro: ", total)
    elif entrada == "0":
