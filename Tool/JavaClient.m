% =========================================================================
% Title         : JavaClient.m
% Author        : Arend Aerts
% Project       : Model-Based testing tool for hybrid systems in Acumen
% Date          : 10-04-2015
% Description   : Run Java client (in combination with Acumen server)
%
%                                ---
%
% Copyright (©) 2010-2014, Acumen Individual Contributors (AIC)
% All rights reserved.
%
% See LICENSE-AIC for licensing details. 
% =========================================================================

function [acumen] = JavaClient(acumen,Matlab_M_Testcase)

% Java client includes
import java.io.*;
import java.net.*;


% Start client socket connection --
try
    sock = Socket('127.0.0.1',5000);
    out = PrintWriter(sock.getOutputStream,true);
    in = BufferedReader(InputStreamReader(sock.getInputStream));
catch ME
    error(ME.identifier, 'Connection Error: %s', ME.message)
end


% Receive data from server --
k=1;                                                                        % Reset variable (End of simulation)
while (k>0)
  str1 = in.readLine();
  %disp(['Server says: ' char(str1)])  % Show JSON data
  [str2,acumen,k]=JSON_update(str1,acumen,Matlab_M_Testcase,k);
  %disp(['Server says: ' char(str2)])  % Show JSON data
  out.println(str2);
end


% Close client socket connection --
out.close();
in.close();
sock.close();


