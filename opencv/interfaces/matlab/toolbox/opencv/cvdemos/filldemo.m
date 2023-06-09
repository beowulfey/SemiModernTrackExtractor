function filldemo(action,varargin);
%   OpenCV cvFloodFill function demo

global demoName;
demoName = 'OpenCV FloodFill demo';
if nargin<1,
   action='InitializeDEMO';
end;

feval(action,varargin{:});
return;


%%%
%%%  Sub-function - InitializeDEMO
%%%

function InitializeDEMO()

global demoName;

% If demo is already running, bring it to the foreground.
h = findobj(allchild(0), 'tag', demoName);
if ~isempty(h)
   figure(h(1))
   return
end

screenD = get(0, 'ScreenDepth');
if screenD>8
   grayres=256;
else
   grayres=128;
end
 
DemoFig=figure( ...
   'Name',demoName, ...
   'NumberTitle','off', 'HandleVisibility', 'on', ...
   'tag', demoName, ...
   'Visible','off', 'Resize', 'off',...
   'BusyAction','Queue','Interruptible','off',...
   'IntegerHandle', 'off', ...
   'Doublebuffer', 'on', ...
   'Colormap', gray(grayres));

%====================================
% Information for all buttons (and menus)
btnWid=0.175;
btnHt=0.06;

%====================================
% The CONSOLE frame
frmBorder=0.02;
frmBottom = 0.13;

%frmLeft = 0.5-btnWid-1.5*frmBorder;
frmLeft = 0.52 + frmBorder;
frmWidth = 2*btnWid+3*frmBorder;
frmHeight = btnHt+2*frmBorder;
frmPos=[frmLeft frmBottom frmWidth frmHeight];
h=uicontrol( ...
   'Parent', DemoFig, ...
   'BusyAction','Queue','Interruptible','off',...
   'Style','frame', ...
   'Units','normalized', ...
   'Position',frmPos, ...
   'BackgroundColor',[0.45 0.45 0.45]);

%====================================
% The Apply button
labelStr='Apply';
callbackStr='filldemo(''Apply'')';
yPos=frmBottom+frmBorder;
applyHndl=uicontrol( ...
   'Parent', DemoFig, ...
   'BusyAction','Queue','Interruptible','off',...
   'Style','pushbutton', ...
   'Units','normalized', ...
   'Position',[frmLeft+frmBorder yPos btnWid btnHt], ...
   'String',labelStr, ...
   'Enable', 'off', ...
   'Callback',callbackStr);


%====================================
% The CLOSE button
labelStr='Close';
callbackStr='close(gcf)';

closeHndl=uicontrol( ...
   'Parent', DemoFig, ...
   'BusyAction','Queue','Interruptible','off',...
   'Style','pushbutton', ...
   'Units','normalized', ...
   'Position',[frmLeft+btnWid+2*frmBorder yPos btnWid btnHt], ...
   'Enable', 'off', ...
   'String',labelStr, ...
   'Callback',callbackStr);

c = get(DemoFig,'Color');
if [.298936021 .58704307445 .114020904255]*c'<.5,
   fgColor = [1 1 1];
else
   fgColor = [0 0 0];
end

%==================================
% Set up the image axes
figpos = get(DemoFig, 'position');
row = figpos(4); col = figpos(3);  % dimensions of figure window

vertSpac = (row-330)/3;
horSpac = (col-2*256)/3;
hSrcAx = axes('Parent', DemoFig, ...
   'units', 'pixels', ...
   'BusyAction','Queue','Interruptible','off',...
   'ydir', 'reverse', ...
   'XLim', [.5 255.5], ...
   'YLim', [.5 255.5],...
   'CLim', [0 255], ...
   'XTick',[],'YTick',[], ...
   'Position', [horSpac row-vertSpac-256 256 256]);
title('Source Image');

hDstAx = axes('Parent', DemoFig, ...
   'units', 'pixels', ...
   'BusyAction','Queue','Interruptible','off',...
   'ydir', 'reverse', ...
   'XLim', [.5 255.5], ...
   'YLim', [.5 255.5],...
   'CLim', [0 255], ...
   'XTick',[],'YTick',[], ...
   'Position', [2 * horSpac + 256 row-vertSpac-256 256 256]);
title('Destination Image');

%==================================
% Set up the images
blank = repmat(uint8(0),256,256);
hSrcImage = image('Parent', hSrcAx,...
   'CData', blank, ...
   'BusyAction','Queue','Interruptible','off',...
   'CDataMapping', 'scaled', ...
   'Xdata', [1 256],...
   'Ydata', [1 256],...
   'ButtonDownFcn', 'filldemo(''SeedPointUpdate'')', ...
   'EraseMode', 'none');

hDstImage = image('Parent', hDstAx,...
   'CData', blank, ...
   'BusyAction','Queue','Interruptible','off',...
   'CDataMapping', 'scaled', ...
   'Xdata', [1 256],...
   'Ydata', [1 256],...
   'EraseMode', 'none');

%==================================
% SeedPoint
seedPoint = [128, 100];
hSeedPoint = [line('Parent', hSrcAx, ...
   'XData', [], ...
   'YData', [], ...
   'UserData', seedPoint), ...
line('Parent', hSrcAx, ...
   'XData', [], ...
   'YData', [])];
%   'EraseMode', 'xor', ...

% Status bar
% rangePos = [64 3 280 15];
rangePos = [0 .01 1 .05];
hStatus = uicontrol( ...
   'Parent', DemoFig, ...
   'BusyAction','Queue','Interruptible','off',...
   'Style','text', ...
   'Units','normalized', ...
   'Position',rangePos, ...
   'Horiz','center', ...
   'Background',c, ...
   'Foreground',[.8 0 0], ...
   'Tag', 'Status', ...
   'String','Status bar');


ctrHt = 19; % controls' height
srcImgPos = get(hSrcAx, 'Position');
%====================================
% The New Value Edit
pos = [srcImgPos(1) + srcImgPos(3) / 2 ...
       srcImgPos(2) - 2 * ctrHt srcImgPos(3) / 2 ctrHt];
hNewValEdit=uicontrol( ...
   'Parent', DemoFig, ...
   'BusyAction','Queue','Interruptible','off',...
   'Style','edit', ...
   'Background','white', ...
   'Foreground','black', ...
   'Units','pixels', ...
   'Position',pos, ...
   'String','255', ...
   'Horiz', 'right',...
   'Tag','AppEdit', ...
   'UserData', [255], ...
   'Callback','filldemo(''ControlsUpdate'')');
%====================================
% The New Value Edit Label
pos = [srcImgPos(1) ...
       srcImgPos(2) - 2 * ctrHt srcImgPos(3) / 2 ctrHt];
h = uicontrol( ...
   'Parent', DemoFig, ...
   'BusyAction','Queue','Interruptible','off',...
   'Style','text', ...
   'Units','pixels', ...
   'Position',pos, ...
   'Horiz','left', ...
   'Background',c, ...
   'Foreground','black', ...
   'String','New Value:');

%====================================
% The loDiff Edit
pos = [srcImgPos(1) + srcImgPos(3) / 2 ...
       srcImgPos(2) - 3 * ctrHt srcImgPos(3) / 2 ctrHt];
hLoDiffEdit=uicontrol( ...
   'Parent', DemoFig, ...
   'BusyAction','Queue','Interruptible','off',...
   'Style','edit', ...
   'Background','white', ...
   'Foreground','black', ...
   'Units','pixels', ...
   'Position',pos, ...
   'String','1', ...
   'Horiz', 'right',...
   'Tag','AppEdit', ...
   'UserData', [1], ...
   'Callback','filldemo(''ControlsUpdate'')');
%====================================
% The loDiff Edit Label
pos = [srcImgPos(1) ...
       srcImgPos(2) - 3 * ctrHt srcImgPos(3) / 2 ctrHt];
h = uicontrol( ...
   'Parent', DemoFig, ...
   'BusyAction','Queue','Interruptible','off',...
   'Style','text', ...
   'Units','pixels', ...
   'Position',pos, ...
   'Horiz','left', ...
   'Background',c, ...
   'Foreground','black', ...
   'String','Lower Diff:');

%====================================
% The upDiff Edit
pos = [srcImgPos(1) + srcImgPos(3) / 2 ...
       srcImgPos(2) - 4 * ctrHt srcImgPos(3) / 2 ctrHt];
hUpDiffEdit=uicontrol( ...
   'Parent', DemoFig, ...
   'BusyAction','Queue','Interruptible','off',...
   'Style','edit', ...
   'Background','white', ...
   'Foreground','black', ...
   'Units','pixels', ...
   'Position',pos, ...
   'String','1', ...
   'Horiz', 'right',...
   'Tag','AppEdit', ...
   'UserData', [1], ...
   'Callback','filldemo(''ControlsUpdate'')');
%====================================
% The upDiff Edit Label
pos = [srcImgPos(1) ...
       srcImgPos(2) - 4 * ctrHt srcImgPos(3) / 2 ctrHt];
h = uicontrol( ...
   'Parent', DemoFig, ...
   'BusyAction','Queue','Interruptible','off',...
   'Style','text', ...
   'Units','pixels', ...
   'Position',pos, ...
   'Horiz','left', ...
   'Background',c, ...
   'Foreground','black', ...
   'String','Upper Diff:');

%====================================
% The Connectivity Popup
pos = [srcImgPos(1) + srcImgPos(3) / 2 ...
       srcImgPos(2) - 5 * ctrHt srcImgPos(3) / 2 ctrHt];
hConPop=uicontrol( ...
   'Parent', DemoFig, ...
   'BusyAction','Queue','Interruptible','off',...
   'Style','popupmenu', ...
   'BackgroundColor',[.8 .8 .8], ...
   'Units','pixels', ...
   'Position',pos, ...
   'String','4|8', ...
   'Tag','ThreshPop', ...
   'Callback','filldemo(''ControlsUpdate'')');
%====================================
% The Connectivity Label
pos = [srcImgPos(1) ...
       srcImgPos(2) - 5 * ctrHt srcImgPos(3) / 2 ctrHt];
h = uicontrol( ...
   'Parent', DemoFig, ...
   'BusyAction','Queue','Interruptible','off',...
   'Style','text', ...
   'Units','pixels', ...
   'Position',pos, ...
   'Horiz','left', ...
   'Background',c, ...
   'Foreground','black', ...
   'String','Connectivity:');

setstatus(DemoFig, 'Initializing Demo...');
set(DemoFig, 'Pointer', 'watch');
drawnow;
set(DemoFig, 'Visible','on');

% Put handles to graphics objects and controls in the figure's userdata
handles.SrcImage = hSrcImage;
handles.DstImage = hDstImage;

handles.SrcAx = hSrcAx;

handles.SeedPoint = hSeedPoint;
handles.NewValEdit = hNewValEdit;
handles.LoDiffEdit = hLoDiffEdit;
handles.UpDiffEdit = hUpDiffEdit;
handles.ConPop     = hConPop;

set(DemoFig, 'UserData', handles);

LoadNewImage(DemoFig);

set(DemoFig, 'HandleVisibility','Callback')
set([closeHndl applyHndl], 'Enable', 'on');
return


%%%
%%%  Sub-Function - LoadNewImage
%%%

function LoadNewImage(DemoFig)

if nargin<1
   DemoFig = gcbf;
end

set(DemoFig,'Pointer','watch');

handles = get(DemoFig,'UserData');

hSrcImage = handles.SrcImage;
hSrcAx    = handles.SrcAx;

alumgrns2 = [];
load cvdemos alumgrns2;

img = alumgrns2;

set(hSrcImage, 'Cdata', img);

blank = repmat(uint8(0),256,256);
set(handles.DstImage, 'Cdata', blank);

drawnow;

ControlsUpdate(DemoFig);
Apply(DemoFig);
return;


%%%
%%%  Sub-Function - Apply
%%%

function Apply(DemoFig)

if nargin<1
   DemoFig = gcbf;
end

handles = get(DemoFig,'UserData');

hSrcImage=handles.SrcImage;
hDstImage=handles.DstImage;

hSeedPoint  = handles.SeedPoint;
hNewValEdit = handles.NewValEdit;
hLoDiffEdit = handles.LoDiffEdit;
hUpDiffEdit = handles.UpDiffEdit;
hConPop     = handles.ConPop;

set(DemoFig,'Pointer','watch');
setstatus(DemoFig, 'FloodFilling image...'); drawnow;

img = get(hSrcImage, 'CData');

seedPoint = get(hSeedPoint(1), 'UserData');
newVal = get(hNewValEdit, 'UserData');
loDiff = get(hLoDiffEdit, 'UserData');
upDiff = get(hUpDiffEdit, 'UserData');

% connectivity value
v = get(hConPop,{'Value','String'});
connectivity = str2num(deblank(v{2}(v{1},:)));

% Flood Fill the image
% [dst area value rect] = cvFloodFill(img, ...
%     [seedPoint(2)-1 seedPoint(1)-1], newVal, ...
%     loDiff, upDiff, connectivity);
[dst area value rect] = cvFloodFill(img, ...
    seedPoint, newVal, ...
    loDiff, upDiff, connectivity);

set(hDstImage, 'CData', dst);

setstatus(DemoFig, 'Done');
set(DemoFig,'Pointer','arrow'); drawnow
return


%%%
%%%  Sub-function - ControlsUpdate
%%%

function ControlsUpdate(DemoFig)

if nargin<1
   DemoFig = gcbf;
end;

handles = get(DemoFig,'UserData');

hSrcImage   = handles.SrcImage;

hSeedPoint  = handles.SeedPoint;
hNewValEdit = handles.NewValEdit;
hLoDiffEdit = handles.LoDiffEdit;
hUpDiffEdit = handles.UpDiffEdit;

% draw seed point
pointSize = 10;

seedPoint = get(hSeedPoint(1), 'UserData');
srcImage = get(hSrcImage, 'CData');
if(srcImage(seedPoint(1),seedPoint(2)) > 128)
    color = 'black';
else
    color = 'white';
end

set(hSeedPoint(1), ...
   'XData', [seedPoint(1)-pointSize, seedPoint(1)+pointSize], ...
   'YData', [seedPoint(2), seedPoint(2)], ...
   'Color', color);
set(hSeedPoint(2), ...
   'XData', [seedPoint(1), seedPoint(1)], ...
   'YData', [seedPoint(2)-pointSize, seedPoint(2)+pointSize], ...
   'Color', color);

% newVal value
oldv = get(hNewValEdit, 'UserData');
newv = str2num(get(hNewValEdit, 'String'));
if isempty(newv) | newv(1) < 0 | newv(1) > 255
    newv = oldv;
end
set(hNewValEdit, 'String', num2str(newv(1)));
set(hNewValEdit, 'UserData', newv(1));

% loDiff value
oldv = get(hLoDiffEdit, 'UserData');
newv = str2num(get(hLoDiffEdit, 'String'));
if isempty(newv) | newv(1) < 0 | newv(1) > 255
    newv = oldv;
end
set(hLoDiffEdit, 'String', num2str(newv(1)));
set(hLoDiffEdit, 'UserData', newv(1));

% upDiff value
oldv = get(hUpDiffEdit, 'UserData');
newv = str2num(get(hUpDiffEdit, 'String'));
if isempty(newv) | newv(1) < 0 | newv(1) > 255
    newv = oldv;
end
set(hUpDiffEdit, 'String', num2str(newv(1)));
set(hUpDiffEdit, 'UserData', newv(1));

setstatus(DemoFig, 'Press ''Apply'' button');
return;

%%%
%%%  Sub-function - SeedPointUpdate
%%%

function SeedPointUpdate(DemoFig)
if nargin<1
   DemoFig = gcbf;
end;

handles = get(DemoFig,'UserData');

hSeedPoint = handles.SeedPoint;
hSrcAx     = handles.SrcAx;
hSrcImage  = handles.SrcImage;

srcImage = get(hSrcImage, 'CData');
imgSize  = size(srcImage);
imgSize  = [imgSize(2) imgSize(1)];
seedPoint = get(hSrcAx, 'CurrentPoint');
seedPoint = max([seedPoint(1, 1) seedPoint(1, 2);1 1]);
seedPoint = floor(min([seedPoint;imgSize]));

set(hSeedPoint(1), 'UserData', seedPoint);

ControlsUpdate(DemoFig);

return;
