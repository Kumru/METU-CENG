`timescale 1ns / 1ps

module lab4_2(
  input [1:0] mode,
  input [2:0] n_gasoline_pumps,      // for setup mode, max 6
  input [2:0] n_diesel_pumps,        // for setup mode, max 6
  input [3:0] fuel_amount,           // for add car mode, max 8
  input fuel_type,                   // for add car mode, gas(0) or diesel(1)
  input CLK, 

  output reg [0:5] pump_status,                // pump is working or not, max 6 pumps
  output reg is_gasoline_queue_not_full,       // gasoline queue full warning
  output reg is_diesel_queue_not_full,         // diesel queue full warning

  output reg [3:0] n_cars_in_gasoline_queue,   // to represent max 9 on 1 7-seg display
  output reg [3:0] n_cars_in_diesel_queue,     // to represent max 9 on 1 7-seg display

  output reg [7:0] total_gasoline_needed,      // to represent max 99 on 2 7-seg displays
  output reg [7:0] total_diesel_needed,        // to represent max 99 on 2 7-seg displays

  // reset these after one clock
  output reg invalid_gasoline_car,             // to lightup dot at an2
  output reg invalid_diesel_car,               // to lightup dot at an0
  output reg invalid_setup_params              // to lightup all dots
);

// write your code here

reg flag;

reg [1:0] isSetupDone;

reg [3:0] pump_status_amounts [0:5]; 

reg [2:0] nGasolinePumps;
reg [2:0] nDieselPumps;

//QUEUE INFO
reg [3:0] diesel_queue [7:0];
reg [3:0] gasoline_queue [7:0];
reg [2:0] diesel_queue_head;
reg [2:0] diesel_queue_tail;
reg [2:0] gasoline_queue_head;
reg [2:0] gasoline_queue_tail;

integer i;

initial
begin
  isSetupDone = 0;
  //OUTPUT RESET
  pump_status = 6'b000000;
  is_gasoline_queue_not_full = 0;
  is_diesel_queue_not_full = 0;
  n_cars_in_gasoline_queue = 4'b1111;
  n_cars_in_diesel_queue = 4'b1111;
  total_gasoline_needed = 8'b11111111;
  total_diesel_needed = 8'b11111111;
  invalid_gasoline_car = 0;
  invalid_diesel_car = 0;
  invalid_setup_params = 0;
end

always @ ( posedge CLK )
begin

  invalid_gasoline_car = 0;
  invalid_diesel_car = 0;
  invalid_setup_params = 0;

  if( mode > 1 && n_diesel_pumps + n_gasoline_pumps < 7 && n_diesel_pumps + n_gasoline_pumps > 0 )      // SETUP
  begin
    isSetupDone = 1;

    //Pump Counts
    nGasolinePumps = n_gasoline_pumps;
    nDieselPumps = n_diesel_pumps;

    //QUEUE POSITION RESET
    diesel_queue_head = 0;
    diesel_queue_tail = 0;
    gasoline_queue_head = 0;
    gasoline_queue_tail = 0;

    // QUEUES AND PUMPS
    pump_status_amounts[0] = 0; pump_status_amounts[1] = 0; pump_status_amounts[2] = 0;
    pump_status_amounts[3] = 0; pump_status_amounts[4] = 0; pump_status_amounts[5] = 0;

    case(nGasolinePumps + nDieselPumps)
    1: pump_status = 6'b100000;
    2: pump_status = 6'b110000;
    3: pump_status = 6'b111000;
    4: pump_status = 6'b111100;
    5: pump_status = 6'b111110;
    6: pump_status = 6'b111111;
    endcase

    gasoline_queue[0] = 0; gasoline_queue[1] = 0; gasoline_queue[2] = 0; gasoline_queue[3] = 0;
    gasoline_queue[4] = 0; gasoline_queue[5] = 0; gasoline_queue[6] = 0; gasoline_queue[7] = 0;
    diesel_queue[0] = 0; diesel_queue[1] = 0; diesel_queue[2] = 0; diesel_queue[3] = 0;
    diesel_queue[4] = 0; diesel_queue[5] = 0; diesel_queue[6] = 0; diesel_queue[7] = 0;

    
    //IF 0 THEN LED IS CLOSED
    if(n_gasoline_pumps == 0)
    begin
      n_cars_in_gasoline_queue = 4'b1111;
      total_gasoline_needed = 8'b11111111;
      is_gasoline_queue_not_full = 0;
    end
    else
    begin
      total_gasoline_needed = 0;
      n_cars_in_gasoline_queue = 0;
      is_gasoline_queue_not_full = 1;
    end
    
    if(n_diesel_pumps == 0)
    begin
      n_cars_in_diesel_queue = 4'b1111;
      total_diesel_needed = 8'b11111111;
      is_diesel_queue_not_full = 0;
    end
    else
    begin
      total_diesel_needed = 0;
      n_cars_in_diesel_queue = 0;
      is_diesel_queue_not_full = 1;
    end

  end
  else if( mode > 1 && (n_diesel_pumps + n_gasoline_pumps > 6 || n_diesel_pumps + n_gasoline_pumps < 1) )
    invalid_setup_params = 1;

  if( mode < 2 && isSetupDone )                                  // SIMULATION
  begin

    // Step (iii) - Serve for one unit time
    for(i=0;i<6;i=i+1)
    begin
      if( pump_status_amounts[i] )
      begin
        if( i < nGasolinePumps )
        begin
          //if( pump_status_amounts[i] == 1 ) pump_status[i] = 1; // Car finished
          pump_status_amounts[i] = pump_status_amounts[i] - 1; // Fuel need decreases 

          total_gasoline_needed = total_gasoline_needed - 1; // Total fuel increases
        end
        else if( i < nGasolinePumps + nDieselPumps )
        begin
          //if( pump_status_amounts[i] == 1 ) pump_status[i] = 0; // Car finished
          pump_status_amounts[i] = pump_status_amounts[i] - 1; // Fuel need decreases 

          total_diesel_needed = total_diesel_needed - 1; // Total fuel increases
        end
      end
    end

    // Step (i)   - Cars from queue to pumps
    for(i=0;i<6;i=i+1)
    begin
      if( pump_status[i] )
      begin
        if( i < nGasolinePumps && n_cars_in_gasoline_queue )
        begin
          pump_status[i] = 0; // Fill the pump
          pump_status_amounts[i] = gasoline_queue[gasoline_queue_head]; // Fill pumps amount
          gasoline_queue[gasoline_queue_head] = 0; // Clear the queue

          if( gasoline_queue_head == 7 ) gasoline_queue_head = 0; // Move the head a step forward
          else gasoline_queue_head = gasoline_queue_head + 1;
          if( is_gasoline_queue_not_full == 0 ) is_gasoline_queue_not_full = 1; // Queue not full

          n_cars_in_gasoline_queue = n_cars_in_gasoline_queue - 1; // Decrement counter
        end
        else if( i >= nGasolinePumps && i < nGasolinePumps + nDieselPumps && n_cars_in_diesel_queue )
        begin
          pump_status[i] = 0; // Fill the pump
          pump_status_amounts[i] = diesel_queue[diesel_queue_head]; // Fill pumps amount
          diesel_queue[diesel_queue_head] = 0; // Clear the queue

          if( diesel_queue_head == 7 ) diesel_queue_head = 0; // Move the head a step forward
          else diesel_queue_head = diesel_queue_head + 1;

          if( is_diesel_queue_not_full == 0 ) is_diesel_queue_not_full = 1; // Queue not full

          n_cars_in_diesel_queue = n_cars_in_diesel_queue - 1; // Decrement counter
        end
      end
    end

    // Step (ii)  - Car entrance
    if( mode == 1 )                                              // CAR ENTRANCE
    begin
      if( fuel_amount > 0 && fuel_amount < 9 ) // VALID AMOUNT
      begin
        flag = 1;
        if( fuel_type == 0 && is_gasoline_queue_not_full == 1 ) // GASOLINE
        begin
          if ( pump_status > 0 && !n_cars_in_gasoline_queue ) // Directly into pump
          begin
            for(i=0;i<6;i=i+1)
            begin
              if(flag)
              begin
                if( i < nGasolinePumps && pump_status[i] )
                begin
                  pump_status[i] = 0;
                  pump_status_amounts[i] = fuel_amount;
                  total_gasoline_needed = total_gasoline_needed + fuel_amount;
                  flag = 0;
                end
              end
            end
          end
          if( flag )
          begin
            gasoline_queue[gasoline_queue_tail] = fuel_amount; // Added
            total_gasoline_needed = total_gasoline_needed + fuel_amount; // Increase total fuel needed
            if( gasoline_queue_tail == 7 ) gasoline_queue_tail = 0; // Move the tail a step forward
            else gasoline_queue_tail = gasoline_queue_tail + 1;
            n_cars_in_gasoline_queue = n_cars_in_gasoline_queue + 1; // Increment counter
            if( n_cars_in_gasoline_queue == 8 ) is_gasoline_queue_not_full = 0; // Got fulled?
          end
        end
        else if( fuel_type == 0 && is_gasoline_queue_not_full == 0 ) invalid_gasoline_car = 1; // Queue full

        if( fuel_type == 1 && is_diesel_queue_not_full == 1 )   // DIESEL
        begin
          if( pump_status > 0 && !n_cars_in_diesel_queue ) // Directly into pump
          begin
            for(i=0;i<6;i=i+1)
            begin
              if(flag)
              begin
                if( i >= nGasolinePumps && i < nGasolinePumps + nDieselPumps && pump_status[i] )
                begin
                  pump_status[i] = 0;
                  pump_status_amounts[i] = fuel_amount;
                  total_diesel_needed = total_diesel_needed + fuel_amount;
                  flag = 0;
                end
              end
            end
          end

          if( flag )
          begin
            diesel_queue[diesel_queue_tail] = fuel_amount; // Added
            total_diesel_needed = total_diesel_needed + fuel_amount; // Increase total fuel needed
            if( diesel_queue_tail == 7 ) diesel_queue_tail = 0; // Move a step forward
            else diesel_queue_tail = diesel_queue_tail + 1;
            n_cars_in_diesel_queue = n_cars_in_diesel_queue + 1; // Increment counter
            if( n_cars_in_diesel_queue == 8 ) is_diesel_queue_not_full = 0; // Got fulled?
          end
        end
        else if( fuel_type == 1 && is_diesel_queue_not_full == 0 ) invalid_diesel_car = 1; // Queue full
      end
      else
      begin                                             // Erroneous input
        if( fuel_type == 0 ) invalid_gasoline_car = 1;
        if( fuel_type == 1 ) invalid_diesel_car = 1;
      end
    end

    if( pump_status[0] == 0 && pump_status_amounts[0] == 0 && 0 < nGasolinePumps+nDieselPumps )
    begin
      pump_status[0] = 1;
      pump_status_amounts [0] = 0;
    end
    if( pump_status[1] == 0 && pump_status_amounts[1] == 0 && 1 < nGasolinePumps+nDieselPumps )
    begin
      pump_status[1] = 1;
      pump_status_amounts [1] = 0;
    end
    if( pump_status[2] == 0 && pump_status_amounts[2] == 0 && 2 < nGasolinePumps+nDieselPumps )
    begin
      pump_status[2] = 1;
      pump_status_amounts [2] = 0;
    end
    if( pump_status[3] == 0 && pump_status_amounts[3] == 0 && 3 < nGasolinePumps+nDieselPumps )
    begin
      pump_status[3] = 1;
      pump_status_amounts [3] = 0;
    end
    if( pump_status[4] == 0 && pump_status_amounts[4] == 0 && 4 < nGasolinePumps+nDieselPumps )
    begin
      pump_status[4] = 1;
      pump_status_amounts [4] = 0;
    end
    if( pump_status[5] == 0 && pump_status_amounts[5] == 0 && 5 < nGasolinePumps+nDieselPumps )
    begin
      pump_status[5] = 1;
      pump_status_amounts [5] = 0;
    end
    
  end
end

endmodule
