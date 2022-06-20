/* verilator lint_off UNUSED */
/* verilator lint_off WIDTH */
/* verilator lint_off MULTIDRIVEN */
module gpioemu(n_reset, saddress[11:0], srd, swr, sdata_in[31:0], sdata_out[31:0],gpio_in[7:0], gpio_latch, gpio_out[7:0], clk, gpio_in_s_insp[31:0]);

	input 			n_reset;
	input [11:0] 	saddress; 
	input 			srd; 
	input 			swr; 
	input [31:0] 	sdata_in; 
	input [7:0] 	gpio_in;
	input 			gpio_latch;
	input 			clk;
	
	
	output [31:0]	sdata_out;
	output [7:0] 	gpio_out;
	output [31:0] 	gpio_in_s_insp;
	
	
	// REJESTRY 
	reg [7:0] 		gpio_in_s;
	reg [7:0]		gpio_out_s;
	reg [31:0] 		sdata_out_s;
	reg [7:0] 		counter;
	reg 			interrupt;



	always @(negedge n_reset)
	begin
		gpio_in_s <= 0;
		gpio_out_s <= 0;
		sdata_out_s <= 0;
		counter <= 8'h56;
		interrupt <= 0;	
	end

	always @(posedge gpio_latch)
		begin
			gpio_in_s <= gpio_in;
		end

	
	// odczyt
	
	always @(posedge srd)
		begin
			if (saddress == 12'h210) //axis 1
				begin
					sdata_out_s <= gpio_in_s[3:0] << 9;
				end
			else if (saddress == 12'h214) //axis 2
				begin
					sdata_out_s <= gpio_in_s[7:4] << 21;
				end
			else if (saddress == 12'h218) //counter
				begin
					sdata_out_s <= interrupt << 5;
				end
			else
				begin
					sdata_out_s <= 0;
				end
		end

	// zapis
	
	always @(posedge swr)
		begin
			if (saddress == 12'h210) //axis 1
				begin
					gpio_out_s[3:0] <= sdata_in[12:9];
				end
			else if (saddress == 12'h214) //axis 2
				begin
					gpio_out_s[7:4] <= sdata_in[24:21];
				end
			else if (saddress == 12'h218) //counter
				begin
					if (sdata_in == 32'h20)
						begin
							interrupt <= 0;
							counter <= 8'h56;
						end
				end
			else
        		begin
            		gpio_out_s <= gpio_out;
        		end
		end


	// licznik
	
	always @(posedge clk)
	begin
		if (counter > 0)
			counter <= counter - 1; 
		else if (counter == 0)
			interrupt <= 1;
	end
	
		
	assign sdata_out = sdata_out_s;
	assign gpio_in_s_insp = gpio_in_s;
	assign gpio_out = gpio_out_s;
	
endmodule
