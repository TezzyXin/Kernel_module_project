module gpioemu_tb;

	reg n_reset;
	
	reg [11:0] saddress;
	
	reg srd;
	reg swr;
	
	reg [31:0] sdata_in;
	wire [31:0] sdata_out;
	
	reg [7:0] gpio_in;
	reg gpio_latch;
	wire [7:0] gpio_out;
	
	reg clk;
	
	wire [31:0] gpio_in_s_insp;
	
	integer i;
	
	initial begin
		$dumpfile("gpioemu.vcd");
		$dumpvars(0, gpioemu_tb);
	end
	
	always 
		# 5 clk = ~clk;
		
	initial begin
		# 5 clk = 0;
		
		# 5 n_reset = 0;
		# 5 n_reset = 1;
		# 5 n_reset = 0;
		
		# 5 srd = 0;
		# 5 swr = 0;
		
		# 0 sdata_in = 0;
		# 0 saddress = 0;
		
		// Oś 1
		
		# 5 gpio_latch = 0;
		# 5 gpio_in = 8'he5; 
		# 5 gpio_latch = 1;
		# 5 gpio_latch = 0;
		
		//// Odczyt
		# 5 saddress = 12'h210;
		# 5 srd = 1;
		# 5 srd = 0;
		
		//// Zapis
		# 5 sdata_in = 32'ha00;
		# 5 srd = 1;
		# 5 srd = 0;

		# 5 swr = 1;
		# 5 swr = 0;
		
		// Oś 2
		
		# 5 gpio_latch = 0;
		# 5 gpio_in = 8'h99; 
		# 5 gpio_latch = 1;
		# 5 gpio_latch = 0;
		
		//// Odczyt
		# 5 saddress = 12'h214;
		# 5 srd = 1;
		# 5 srd = 0;
		
		//// Zapis
		# 5 sdata_in = 32'h1200000;
		# 5 srd = 1;
		# 5 srd = 0;

		# 5 swr = 1;
		# 5 swr = 0;
		
		// Licznik
		//// Odczyt
		# 5 saddress = 12'h218;
		# 5 srd = 1;
		# 5 srd = 0;
		
		//// Zapis
		# 5 sdata_in = 32'h020;
		# 5 srd = 1;
		# 5 srd = 0;
		# 5 swr = 1;
		# 5 swr = 0;
				
		
		// Test neutralności
		
		# 5 gpio_in = 8'hab; 
		# 5 gpio_latch = 1;
		# 5 gpio_latch = 0;
		
		# 5 saddress = 16'hbcda;
		# 5 srd = 1;
		# 5 srd = 0;
		
		# 5 saddress = 16'ha404;
		# 5 srd = 1;
		# 5 srd = 0;
		
		# 5 saddress = 16'h5099;
		# 5 srd = 1;
		# 5 srd = 0;
		
		# 5 saddress = 16'h1166;
		# 5 srd = 1;
		# 5 srd = 0;
		
		# 5 saddress = 16'hf001;
		# 5 srd = 1;
		# 5 srd = 0;
		
		# 5 saddress = 16'h8008;
		# 5 srd = 1;
		# 5 srd = 0;
		
		# 1000 $finish;
	end
	
	gpioemu tt(n_reset, saddress, srd, swr, sdata_in, sdata_out, 
	gpio_in, gpio_latch, gpio_out, clk, gpio_in_s_insp);
	

endmodule

