function varargout = gui(varargin)
% GUI MATLAB code for gui.fig
%      GUI, by itself, creates a new GUI or raises the existing
%      singleton*.
%
%      H = GUI returns the handle to a new GUI or the handle to
%      the existing singleton*.
%
%      GUI('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in GUI.M with the given input arguments.
%
%      GUI('Property','Value',...) creates a new GUI or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before gui_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to gui_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to readme gui

% Last Modified by GUIDE v2.5 22-Jan-2018 14:28:02

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @gui_OpeningFcn, ...
                   'gui_OutputFcn',  @gui_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before gui is made visible.
function gui_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to gui (see VARARGIN)

% Define workspace
cd('C:\Users\s132652\Google Drive\TUe\2017-2018\Internship\GIT\cps-conf-tool');

% Define location Acumen .jar file (runfile)
% Note: tooling only tested with acumen-14.12.01.jar release!
handles.acumen_path = 'C:\Users\s132652\Google Drive\TUe\2017-2018\Internship\GIT\cps-conf-tool\Acumen\acumen-14.12.01.jar';

% Define location for CORA installation folder
handles.cora_path = 'C:\Users\s132652\Google Drive\TUe\2017-2018\Internship\GIT\cps-conf-tool\CORA';


% INIT GUI structure
handles.useraction1 = 0;
handles.useraction2 = 0;
set(handles.popupmenu25,'Enable','off');
set(handles.popupmenu_toolingmode,'Enable','off');
set(handles.plotvar,'Enable','off');
set(handles.selectoutput,'Enable','off');
set(handles.popupmenu9,'Enable','off');
set(handles.edit11,'Enable','off');
set(handles.edit12,'Enable','off');
set(handles.edit13,'Enable','off');

set(handles.pushbutton12,'Enable','off');
set(handles.pushbutton13,'Enable','off');
set(handles.start_analysis,'Enable','off');
set(handles.pushbutton14,'Enable','off');
set(handles.pushbutton15,'Enable','off');
set(handles.pushbutton16,'Enable','off');

% INIT data structure
handles.model_path = '';
handles.implementation_path = '';

handles.conformance.var = {};            
handles.conformance.epsilon = [];          
handles.conformance.tau = [];  

handles.acumen.time = [];
handles.acumen.timestep = 0;
handles.acumen.endtime = 0;
handles.acumen.var = {};
handles.acumen.instances = 0;

% handles.matlab.model.time = [];

% handles.matlab.implementation.time = [];

handles.testcase.time = [];
handles.testcase.inputvalue = [];
handles.testcase.inputvar = {};    

% Load Leicester University logo
axes(handles.logo_leicester);
image(100,100,imread('logo_leicester.jpg'));
axis(handles.logo_leicester, 'off')

% Load Universidade Federal de Pernambuco logo
axes(handles.logo_ufp);
image(100,100,imread('logo_ufp.jpg'));
axis(handles.logo_ufp, 'off')

% Load Leicester University logo
axes(handles.logo_leicester);
image(100,100,imread('logo_leicester.jpg'));
axis(handles.logo_leicester, 'off')

% Load TUE LOGO
axes(handles.logo_tue);
image(100,100,imread('logo_tue.jpg'));
axis(handles.logo_tue, 'off')

% Load Halmstad University logo
axes(handles.logo_hu);
image(100,100,imread('logo_hu.jpg'));
axis(handles.logo_hu, 'off')

% Determine tooling mode
tooling_mode = get(handles.popupmenu_toolingmode,'Value');

% Choose default command line output for gui
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes gui wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = gui_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in start_analysis.
function start_analysis_Callback(hObject, eventdata, handles)
% hObject    handle to start_analysis (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Clear axis
cla(handles.axes1);

% Check for performed test case execution
if handles.useraction1 == 1 && handles.useraction2 == 1
% if strcmp(get(handles.pushbutton13,'Enable'),'off') && handles.useraction1 == 1 && handles.useraction2 == 1

    % Determine tooling mode
    handles.tooling_mode = get(handles.popupmenu_toolingmode,'Value');

    if (handles.tooling_mode == 2 && handles.acumen.instances > 1) || handles.tooling_mode == 1
        % STATUS UPDATE GUI 
        temp=get(handles.statup,'String');
        set(handles.statup,'String',strvcat(strcat(datestr(now),'   [MATLAB] (T,t,e)-CLOSENESS check'),char(temp))); 
        drawnow;

        set(handles.text11,'Visible','off');
        set(handles.text13,'Visible','off');
        set(handles.popupmenu9,'String',strvcat(char([cellstr('Overview');cellstr('Zoom')])));
        drawnow; 

        % Run conformance check
        run('TJte_closeness2.m');

        % STATUS UPDATE GUI 
        if conformance.result == 1
            disp(strcat(datestr(now),'   [MATLAB] Conformance check:','   SUCCESFULL'));
            set(handles.text11,'Visible','off');
            set(handles.text13,'Visible','on');
            cla(handles.axes1)
            drawnow;
        else
            disp(strcat(datestr(now),'   [MATLAB] Conformance check:','   FAILED'));
            set(handles.text11,'Visible','on');
            set(handles.text13,'Visible','off');
            drawnow;
        end

        % Update menu structure
        set(handles.popupmenu9,'Enable','on');   
    
    elseif handles.tooling_mode == 3
%         disp('Conformance testing Matlab model and implementation');
        set(handles.popupmenu9,'String',strvcat(char([cellstr('Overview');cellstr('Zoom')])));
        run('TJte_closeness2.m');
        set(handles.popupmenu9,'Enable','on'); 
        
        if conformance.result == 1
            disp(strcat(datestr(now),'   [MATLAB] Conformance check:','   SUCCESFULL'));
            set(handles.text11,'Visible','off');
            set(handles.text13,'Visible','on');
            cla(handles.axes1)
            drawnow;
        else
            disp(strcat(datestr(now),'   [MATLAB] Conformance check:','   FAILED'));
            set(handles.text11,'Visible','on');
            set(handles.text13,'Visible','off');
            drawnow;
        end
        
    else
        % Delete graph
        cla(handles.axes1);

        
    end
end



% --------------------------------------------------------------------
function Untitled_1_Callback(hObject, eventdata, handles)
% --------------------------------------------------------------------
function Untitled_2_Callback(hObject, eventdata, handles)
% --- Executes on selection change in listbox1.
function listbox1_Callback(hObject, eventdata, handles)
% --- Executes during object creation, after setting all properties.
function listbox1_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
% --- Executes on selection change in popupmenu2.
function popupmenu2_Callback(hObject, eventdata, handles)
% --- Executes during object creation, after setting all properties.
function popupmenu2_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function edit3_Callback(hObject, eventdata, handles)
% --- Executes during object creation, after setting all properties.
function edit3_CreateFcn(hObject, eventdata, handles)

if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in select_model.
function select_model_Callback(hObject, eventdata, handles)
% hObject    handle to select_model (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

[filename,filepath] = uigetfile('*.m','Select the MATLAB system model .m file');
handles.model_path=strcat(filepath,filename);

if filename ~= 0    % only if a filename is selected
    % Show the name of the the selected model
    set(handles.text30,'Visible','off');
    set(handles.text31,'String',filename);
    set(handles.text31,'Visible','on');
end

% STATUS UPDATE GUI 
temp=get(handles.statup,'String');
set(handles.statup,'String',strvcat(strcat(datestr(now),'   [MATLAB] Model loaded'),char(temp))); 
drawnow;

% Update menu structure
set(handles.popupmenu_toolingmode,'Enable','on');
set(handles.pushbutton14,'Enable','on');
set(handles.pushbutton16,'Enable','on');
set(handles.pushbutton13,'Enable','on');
if ~isempty(handles.implementation_path) && ~isempty(handles.model_path) 
    set(handles.pushbutton12,'Enable','on');   
end

% Update handles structure
guidata(hObject, handles);

% --- Executes on button press in select_implementation.
function select_implementation_Callback(hObject, eventdata, handles)
% hObject    handle to select_implementation (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

[filename,filepath] = uigetfile('*.acm','Select the system implementation .ACM file');
handles.implementation_path = strcat(filepath,filename);

if filename ~= 0    % only if a filename is selected
    % Show the name of the the selected implementation
    set(handles.text32,'Visible','off');
    set(handles.text33,'String',filename);
    set(handles.text33,'Visible','on'); 
end

% Import input, system and sim variables
run('ACMfile_import2.m');

% Upload variables to plot and comformance selection
handles.acumen.var
set(handles.plotvar,'String',char(handles.acumen.var'));
set(handles.popupmenu25,'String',char(handles.acumen.var'));

% STATUS UPDATE GUI 
temp=get(handles.statup,'String');
set(handles.statup,'String',strvcat(strcat(datestr(now),'   [MATLAB] Implementation loaded'),char(temp))); 
drawnow;

% Update menu structure
if isempty(handles.model_path) 
    handles.tooling_mode = 2;
    set(handles.popupmenu_toolingmode,'Enable','off');
end
set(handles.pushbutton13,'Enable','on');
set(handles.pushbutton14,'Enable','on');

if ~isempty(handles.implementation_path) && ~isempty(handles.model_path) 
    set(handles.pushbutton12,'Enable','on');    
end

% Update handles structure
guidata(hObject, handles);


% --- Executes on selection change in plotvar.
function plotvar_Callback(hObject, eventdata, handles)
% hObject    handle to plotvar (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns plotvar contents as cell array
%        contents{get(hObject,'Value')} returns selected item from plotvar

% Determine tooling mode
tooling_mode = get(handles.popupmenu_toolingmode,'Value');

% Plot results
cla(handles.axes1);
axes(handles.axes1);
if tooling_mode == 1 || tooling_mode == 2
    run('acumen_plot2.m');
elseif tooling_mode == 3
    run('matlab_plot.m');
end

% STATUS UPDATE GUI
temp=get(handles.statup,'String');
set(handles.statup,'String',strvcat(strcat(datestr(now),'   [MATLAB] PLOT:',selected_var),char(temp))); 
drawnow;



% --- Executes during object creation, after setting all properties.
function plotvar_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in temp.
function selectoutput_Callback(hObject, eventdata, handles)
% hObject    handle to temp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns temp contents as cell array
%        contents{get(hObject,'Value')} returns selected item from temp

tooling_mode = get(handles.popupmenu_toolingmode,'Value');

% Plot results
cla(handles.axes1);
axes(handles.axes1);
if tooling_mode == 1 || tooling_mode == 2
    run('acumen_plot2.m');
elseif tooling_mode == 3
    run('matlab_plot.m');
end


% STATUS UPDATE GUI 
temp=get(handles.statup,'String');
set(handles.statup,'String',strvcat(strcat(datestr(now),'   [MATLAB] PLOT:',selected_var),char(temp))); 
drawnow;



% --- Executes during object creation, after setting all properties.
function selectoutput_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
% --- Executes on selection change in statup.
function statup_Callback(hObject, eventdata, handles)
% --- Executes during object creation, after setting all properties.
function statup_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton12.
function pushbutton12_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton12 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Check for existing matlab file path
if ~isempty(handles.model_path)
    temp=get(handles.statup,'String');
    set(handles.statup,'String',strvcat(strcat(datestr(now),'   [MATLAB] Run test case generation'),char(temp))); 
    drawnow;

    % Run test case generation file
    run(handles.model_path);

    % Update menu structure
    set(handles.pushbutton12,'Enable','off');
        
    % Plot results
    axes(handles.axes1);
    run('acumen_plot2.m');

    % Update menu structure
    set(handles.plotvar,'Enable','on');
    set(handles.selectoutput,'Enable','on');
    
    % Update handles structure
    guidata(hObject, handles);
end



% --- Executes on selection change in popupmenu9.
function popupmenu9_Callback(hObject, eventdata, handles)

% Clear axis
cla(handles.axes1);

% Determine tooling mode
handles.tooling_mode = get(handles.popupmenu_toolingmode,'Value');

if (handles.tooling_mode == 2 && handles.acumen.instances > 1) || handles.tooling_mode == 1
    % STATUS UPDATE GUI 
    temp=get(handles.statup,'String');
    set(handles.statup,'String',strvcat(strcat(datestr(now),'   [MATLAB] (T,t,e)-CLOSENESS check'),char(temp))); 
    drawnow;

    set(handles.text11,'Visible','off');
    set(handles.text13,'Visible','off');
    set(handles.popupmenu9,'String',strvcat(char([cellstr('Overview');cellstr('Zoom')])));
    drawnow; 

    % Run conformance check
    run('TJte_closeness2.m');

    % STATUS UPDATE GUI 
    if conformance.result == 1
        disp(strcat(datestr(now),'   [MATLAB] Conformance check:','   SUCCESFULL'));
        set(handles.text11,'Visible','off');
        set(handles.text13,'Visible','on');
        cla(handles.axes1)
        drawnow;
    else
        disp(strcat(datestr(now),'   [MATLAB] Conformance check:','   FAILED'));
        set(handles.text11,'Visible','on');
        set(handles.text13,'Visible','off');
        drawnow;
    end

    % Update menu structure
    set(handles.popupmenu9,'Enable','on');   
else
    % Delete graph
    cla(handles.axes1);
end


% --- Executes during object creation, after setting all properties.
function popupmenu9_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton13.
function pushbutton13_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton13 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

handles.tooling_mode = get(handles.popupmenu_toolingmode,'Value');
tooling_mode = handles.tooling_mode;

if handles.tooling_mode == 1 || handles.tooling_mode == 2

    % Check for existing implementation path
    if ~isempty(handles.implementation_path)

        % STATUS UPDATE GUI 
        temp=get(handles.statup,'String');
        set(handles.statup,'String',strvcat(strcat(datestr(now),'   [Acumen] Start / load model file'),char(temp))); 
        drawnow;
        temp=get(handles.statup,'String');
        set(handles.statup,'String',strvcat(strcat(datestr(now),'   [Acumen] Start server mode'),char(temp))); 
        drawnow;
        temp=get(handles.statup,'String');
        set(handles.statup,'String',strvcat(strcat(datestr(now),'   [MATLAB] Run JAVA client'),char(temp))); 
        drawnow;

        % Clear graph
        cla(handles.axes1);

        % Run acumen simulation
        run('acumen_sim2.m');

        % Update menu structure
        set(handles.pushbutton13,'Enable','off');

        % Determine tooling mode
        handles.tooling_mode = get(handles.popupmenu_toolingmode,'Value');

        % Plot results
        axes(handles.axes1);
        run('acumen_plot2.m');

        % STATUS UPDATE GUI 
        temp=get(handles.statup,'String');
        set(handles.statup,'String',strvcat(strcat(datestr(now),'   PLOT:',selected_var),char(temp))); 
        drawnow;

        % Update handles structure
        guidata(hObject, handles);

        % Update menu structure
        set(handles.plotvar,'Enable','on');
        set(handles.selectoutput,'Enable','on');
        set(handles.popupmenu25,'Enable','on');
        set(handles.edit11,'Enable','on');
        set(handles.edit12,'Enable','on');
        set(handles.edit13,'Enable','on');
        set(handles.start_analysis,'Enable','on');
        set(handles.pushbutton15,'Enable','on');
        
    end

elseif handles.tooling_mode == 3
    temp=get(handles.statup,'String');
    set(handles.statup,'String',strvcat(strcat(datestr(now),'   [MATLAB] Loading data from model and implementation'),char(temp))); 
    drawnow;

    % Run test case generation file
    run(handles.model_path);
    
%     handles.matlab.model = model;
%     handles.matlab.implementation = implementation;
    
    % Update menu structure
    set(handles.plotvar,'String',char(handles.matlab.model.variables));
    set(handles.popupmenu25,'String',char(handles.matlab.model.variables));
    
    set(handles.plotvar,'Enable','on');
    set(handles.selectoutput,'Enable','on');
    set(handles.popupmenu25,'Enable','on');
    set(handles.edit11,'Enable','on');
    set(handles.edit12,'Enable','on');
    set(handles.edit13,'Enable','on');
    set(handles.start_analysis,'Enable','on');
    
    % Plot results
    cla(handles.axes1);
    axes(handles.axes1);
    run('matlab_plot.m');

    % Update menu structure
    set(handles.pushbutton16,'Enable','off');
    
    % Update handles structure
    guidata(hObject, handles);
    
end




% --- Executes during object creation, after setting all properties.
function logo_tue_CreateFcn(hObject, eventdata, handles)
% --- Executes during object creation, after setting all properties.
function logo_hu_CreateFcn(hObject, eventdata, handles)
% --------------------------------------------------------------------
function README_Callback(hObject, eventdata, handles)




% --------------------------------------------------------------------
function License_Callback(hObject, eventdata, handles)
% hObject    handle to License (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

%# read text file lines as cell array of strings
fid = fopen('LICENSE-AIC.txt');
str = textscan(fid, '%s', 'Delimiter','\n'); str = str{1};
fclose(fid);

%# GUI with multi-line editbox
hFig = figure('Menubar','none', 'Toolbar','none');
hPan = uipanel(hFig, 'Title','LICENSE', ...
    'Units','normalized', 'Position',[0.05 0.05 0.9 0.9]);
hEdit = uicontrol(hPan, 'Style','edit', 'FontSize',9, ...
    'Min',0, 'Max',2, 'HorizontalAlignment','left', ...
    'Units','normalized', 'Position',[0 0 1 1], ...
    'String',str);

%# enable horizontal scrolling
jEdit = findjobj(hEdit);
jEditbox = jEdit.getViewport().getComponent(0);
jEditbox.setWrapping(false);                %# turn off word-wrapping
jEditbox.setEditable(false);                %# non-editable
set(jEdit,'HorizontalScrollBarPolicy',30);  %# HORIZONTAL_SCROLLBAR_AS_NEEDED

%# maintain horizontal scrollbar policy which reverts back on component resize 
hjEdit = handle(jEdit,'CallbackProperties');
set(hjEdit, 'ComponentResizedCallback',...
    'set(gcbo,''HorizontalScrollBarPolicy'',30)')

% --------------------------------------------------------------------
function About_Callback(hObject, eventdata, handles)
% hObject    handle to About (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

str = { '  '
        'Model-Based Testing tool for Hybrid Systems in Acumen';
        '  ';
        'Version 1.2';
        '  ';
        '  ';
        'Author 1';
        '=======';
        'Arend Aerts';
        'Student Systems and Control';
        'Eindhoven University of Technology';
        'The Netherlands';
        '';
        'Email:  a.aerts@student.tue.nl';
        '  ';
        '  ';
        'Author 2';
        '=======';
        'Dr. Mohammad Reza Mousavi';
        'Centre for Research on Embedded Systems (CERES)';
        'Halmstad University,';
        'Halmstad, Sweden';
        '';
        'Email:  m.r.mousavi@hh.se';        
      };
        

%# GUI with multi-line editbox
hFig = figure('Menubar','none', 'Toolbar','none');
hPan = uipanel(hFig, 'Title','ABOUT', ...
    'Units','normalized', 'Position',[0.05 0.05 0.9 0.9]);
hEdit = uicontrol(hPan, 'Style','edit', 'FontSize',9, ...
    'Min',0, 'Max',2, 'HorizontalAlignment','left', ...
    'Units','normalized', 'Position',[0 0 1 1], ...
    'String',str);

%# enable horizontal scrolling
jEdit = findjobj(hEdit);
jEditbox = jEdit.getViewport().getComponent(0);
jEditbox.setWrapping(false);                %# turn off word-wrapping
jEditbox.setEditable(false);                %# non-editable
set(jEdit,'HorizontalScrollBarPolicy',30);  %# HORIZONTAL_SCROLLBAR_AS_NEEDED

%# maintain horizontal scrollbar policy which reverts back on component resize 
hjEdit = handle(jEdit,'CallbackProperties');
set(hjEdit, 'ComponentResizedCallback',...
    'set(gcbo,''HorizontalScrollBarPolicy'',30)')



function edit11_Callback(hObject, eventdata, handles)
% hObject    handle to edit11 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit11 as text
%        str2double(get(hObject,'String')) returns contents of edit11 as a double

handles.useraction1 = 1;

% Update handles structure
guidata(hObject, handles);



% --- Executes during object creation, after setting all properties.
function edit11_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit11 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


function edit12_Callback(hObject, eventdata, handles)
% hObject    handle to edit12 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit12 as text
%        str2double(get(hObject,'String')) returns contents of edit12 as a double

handles.useraction2 = 1;

% Update handles structure
guidata(hObject, handles);



% --- Executes during object creation, after setting all properties.
function edit12_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit12 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes during object creation, after setting all properties.
function axes8_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axes8 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: place code in OpeningFcn to populate axes8


% --- Executes on selection change in popupmenu_toolingmode.
function popupmenu_toolingmode_Callback(hObject, eventdata, handles)
% hObject    handle to popupmenu_toolingmode (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns popupmenu_toolingmode contents as cell array
%        contents{get(hObject,'Value')} returns selected item from popupmenu_toolingmode

handles.tooling_mode = get(handles.popupmenu_toolingmode,'Value');

% Plot results
cla(handles.axes1);


% Update handles structure
guidata(hObject, handles);


% --- Executes during object creation, after setting all properties.
function popupmenu_toolingmode_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton14.
function pushbutton14_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton14 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Delete graph
cla(handles.axes1);

% RESET data structure
handles.implementation_path = '';
handles.model_path = '';

handles.conformance.var = {};            
handles.conformance.epsilon = [];          
handles.conformance.tau = [];  

handles.acumen.time = [];
handles.acumen.timestep = 0;
handles.acumen.endtime = 0;
handles.acumen.var = {};
handles.acumen.instances = 0;

handles.testcase.time = [];
handles.testcase.inputvalue = [];
handles.testcase.inputvar = {};    

% Reset model and implementation names in file load
set(handles.text30,'Visible','on');
set(handles.text32,'Visible','on');
set(handles.text31,'Visible','off');
set(handles.text33,'Visible','off');

% Update menu structure
handles.useraction1 = 0;
handles.useraction2 = 0;
set(handles.plotvar,'String','System variables...');
set(handles.popupmenu9,'String','Views...');
set(handles.popupmenu25,'String','System variables...');
set(handles.statup,'String',char({'  ';
                                  '  ';
                                  '  ';
                                  '  ';
                                  'Copyright (©) 2010-2014, Acumen Individual Contributors (AIC). All rights reserved.';
                                  '  ';
                                  'Please see License file for licensing details. '}));

set(handles.text11,'Visible','off');
set(handles.text13,'Visible','off');

set(handles.popupmenu25,'Enable','off');
set(handles.popupmenu_toolingmode,'Enable','off');
set(handles.plotvar,'Enable','off');
set(handles.selectoutput,'Enable','off');
set(handles.popupmenu9,'Enable','off');
set(handles.edit11,'Enable','off');
set(handles.edit12,'Enable','off');
set(handles.pushbutton12,'Enable','off');
set(handles.pushbutton13,'Enable','off');
set(handles.pushbutton14,'Enable','off');
set(handles.start_analysis,'Enable','off'); 
set(handles.pushbutton16,'Enable','off');

% --- Executes on selection change in popupmenu25.
function popupmenu25_Callback(hObject, eventdata, handles)
% --- Executes during object creation, after setting all properties.
function popupmenu25_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton15.
function pushbutton15_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton15 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
temp=get(handles.statup,'String');
value = get(handles.edit12,'String');
delta = coraInterface;
set(handles.edit13,'String',delta);




function edit13_Callback(hObject, eventdata, handles)
% hObject    handle to edit13 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit13 as text
%        str2double(get(hObject,'String')) returns contents of edit13 as a double


% --- Executes during object creation, after setting all properties.
function edit13_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit13 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton16.
function pushbutton16_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton16 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
tooling_mode = get(handles.popupmenu_toolingmode,'Value');

if ~isempty(handles.model_path) && tooling_mode == 3
    temp=get(handles.statup,'String');
    set(handles.statup,'String',strvcat(strcat(datestr(now),'   [MATLAB] Loading data from model and implementation'),char(temp))); 
    drawnow;

    % Run test case generation file
    run(handles.model_path);
    
%     handles.matlab.model = model;
%     handles.matlab.implementation = implementation;
    
    % Update menu structure
    set(handles.plotvar,'String',char(handles.matlab.model.variables));
    set(handles.popupmenu25,'String',char(handles.matlab.model.variables));
    
    set(handles.plotvar,'Enable','on');
    set(handles.selectoutput,'Enable','on');
    set(handles.popupmenu25,'Enable','on');
    set(handles.edit11,'Enable','on');
    set(handles.edit12,'Enable','on');
    set(handles.edit13,'Enable','on');
    set(handles.start_analysis,'Enable','on');
    
    % Plot results
    cla(handles.axes1);
    axes(handles.axes1);
    run('matlab_plot.m');

    % Update menu structure
    set(handles.pushbutton16,'Enable','off');
    
    % Update handles structure
    guidata(hObject, handles);
end
