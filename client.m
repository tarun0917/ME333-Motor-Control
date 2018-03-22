function client(port)
%   provides a menu for accessing PIC32 motor control functions
%

%
%   Input Arguments:
%       port - the name of the com port.  This should be the same as what
%               you use in screen or putty in quotes ' '
%q
%   Example:client('/dev/
%       client('/dev/ttyUSB0') (Linux/Mac)
%       client('COM3') (PC)
%
%   For convenience, you may want to change this so that the port is hardcoded.

% Opening COM connection
if ~ isempty(instrfind)
    fclose(instrfind);
    delete(instrfind);
end

fprintf('Opening port %s....\n', port);

% settings for opening the serial port. baud rate 230400, hardware flow control
% wait up to 120 seconds for data before timing out
mySerial = serial(port, 'BaudRate', 230400, 'FlowControl', 'hardware', 'Timeout', 120);
% opens serial connection
fopen(mySerial);
% closes serial port when function exits
clean = onCleanup(@()fclose(mySerial));

has_quit = false;

% menu loop
while ~ has_quit
    fprintf('PIC32 MOTOR DRIVER INTERFACE\n\n');
    % display the menu options; this list will grow
    fprintf('a: Read current sensor (counts) b: Read current sensor (mA) \n');
    fprintf('c: Read encoder (counts)        d: Read encoder (deg)\n');
    fprintf('e: reset encoder                f: Set PWM (-100 to 100) \n');
    fprintf('g: Set Current Gains            h: Get Current Gains\n');
    fprintf('i: Set Position Gains           j: Get Position Gains\n');
    fprintf('k: Test Current Control         l: Go to angle (deg)\n');
    fprintf('m: Load Step Trajectory         n: Load Cubic Trajectory\n');
    fprintf('o: Execute Trajectory           p: Unpower the Motor\n');
    fprintf('q: Quit                         r: Get mode\n\n');
    % read the user's choice
    selection = input('\nENTER COMMAND: ', 's');
 
    % send the command to the PIC32
    fprintf(mySerial, '%c\n', selection);
 
    % take the appropriate action
    switch selection
        case 'a'
            adc_counts = fscanf(mySerial, '%d');
            fprintf('The ADC counts is %d\n', adc_counts)
         
        case 'b'
            adc_ma = fscanf(mySerial, '%d');
            fprintf('The current is %d mA\n', adc_ma)
        case 'c'
            motor_counts = fscanf(mySerial, '%d');
            fprintf('The motor angle is %d counts.\n', motor_counts)
        case 'd' % example operation
            motor_degree = fscanf(mySerial, '%d');
            fprintf('The motor angle is %d degrees.\n', motor_degree)
         
        case 'e'
            motor_reset = fscanf(mySerial, '%d');
            fprintf('The motor angle is %d counts.It is reset\n', motor_reset)
         
        case 'f'
            value = input('\nENTER COMMAND: ', 's');
            fprintf(mySerial, '%s\n', value);
            pwm = fscanf(mySerial, '%f');
            fprintf('PWM has been set to %f\n', pwm)
        case 'g'
            kpki = input('Set in the form [Kp Ki]: ');
            fprintf(mySerial, '%f %f\n', kpki);
        case 'h'
            a = fscanf(mySerial, '%f %f');
            fprintf(['Kp: ', num2str(a(1)), ' Ki: ', num2str(a(2)), ' \n']);
        case 'i'
            kpmkim = input('Set in the form [Kpm Kdm Kim]: ');
            fprintf(mySerial, '%f %f %f\n', kpmkim);
         
        case 'j'
            b = fscanf(mySerial, '%f %f %f');
            fprintf(['Motor Kp: ', num2str(b(1)), '\tMotor Kd: ', num2str(b(2)), '\tMotor Ki: ', num2str(b(3)), ' \n']);
        case 'k'
            fprintf('ITEST Mode\n');
            data = read_plot_matrix(mySerial);
            plot(data);
        case 'l'
            motorangle = input('\nENTER COMMAND: ', 's');
            fprintf(mySerial, '%s\n', motorangle);
         
        case 'm'
            reflist = input('Enter the Values as [time1 angle1 ; time2 angle2 ; ...]: ');
              ref = genRef(reflist, 'step');
            fprintf('Length is : %d\n', length(ref));
            fprintf(mySerial, '%d\n', length(ref));
            for i = 1:length(ref)
                fprintf(mySerial, '%f\n', ref(i));
            end
        case 'n'
          reflist = input('Enter the Values as [time1 angle1 ; time2 angle2 ; ...]: ');
            ref = genRef(reflist, 'cubic');
          fprintf('Length is : %d\n', length(ref));
          fprintf(mySerial, '%d\n', length(ref));
          for i = 1:length(ref)
              fprintf(mySerial, '%f\n', ref(i));
          end
        case 'o'
          data = read_plot_matrix(mySerial);
          plot(data);
       
        case 'p'
          motor_unpower = fscanf(mySerial, '%d');
          fprintf('PWM has been set to %d\n', motor_unpower)
        case 'q'
          has_quit = true; % exit client
        case 'r'
          get_mode = fscanf(mySerial, '%d');
          fprintf('%d\n', get_mode)
        otherwise
          fprintf('Invalid Selection %c\n', selection);
    end
   end
 
end