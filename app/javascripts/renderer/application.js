const { ipcRenderer } = require('electron');
const $ = require('jquery');

ipcRenderer.on('loaded', function(event, data) {
  document.getElementById('title').innerHTML = data.appName + ' App';
  document.getElementById('details').innerHTML = 'built with Electron v' + data.electronVersion;
  document.getElementById('versions').innerHTML = 'running on Node v' + data.nodeVersion + ' and Chromium v' + data.chromiumVersion;

  // Prepare logs table - temporarily here
  $('body').append(`
    <div id="logsTableContainer">
      <button id="logsButtonClear">Clear</button>

      <button id="logsButtonError"><input type="checkbox" checked>ERROR</input></button>
      <button id="logsButtonWarning"><input type="checkbox" checked>WARNING</input></button>
      <button id="logsButtonInfo"><input type="checkbox" checked>INFO</input></button>
      <button id="logsButtonDebug"><input type="checkbox" checked>DEBUG</input></button>


      <button id="logsButtonDetach">Swap</button>

      <table id="logsTable">
      	<thead>
      		<th id="logsTableHeader">Timestamp</th>
      		<th id="logsTableHeader">Level</th>
      		<th id="logsTableHeader">Text</th>
      	</thead>
      	<tbody></tbody>
    	</table>
    </div>
    `);

    var hideWithClass = function(buttonClass, rowClass) {
      if ($(buttonClass + " input")[0].checked) {
        $(rowClass).show();
      } else {
        $(rowClass).hide();
      }
    };

    // Actions --------
    $("#logsButtonClear").on("click", function(){
      $("#logsTable tbody").empty();
    });

    $("#logsButtonDetach").on("click", function(){
      ipcRenderer.send('logs_detached', $("#logsTableContainer").detach());
    });

    $("#logsButtonError").on("click",   function(){ hideWithClass("#logsButtonError",   ".logsTableError") });
    $("#logsButtonWarning").on("click", function(){ hideWithClass("#logsButtonWarning", ".logsTableWarning") });
    $("#logsButtonInfo").on("click",    function(){ hideWithClass("#logsButtonInfo",    ".logsTableInfo") });
    $("#logsButtonDebug").on("click",   function(){ hideWithClass("#logsButtonDebug",   ".logsTableDebug") });
});

ipcRenderer.on('log_line', function(event, data) {
  var levelClass = "";

  switch(data.level){
    case "DEBUG":
      levelClass = "logsTableDebug";
      break;

    case "WARNING":
      levelClass = "logsTableWarning";
      break;

    case "INFO":
      levelClass = "logsTableInfo";
      break;

    case "ERROR":
      levelClass = "logsTableError";
      break;
  }

	$('#logsTable tbody').append(
		`<tr class="`+ levelClass +`">
			<td class="logsTableTimestamp logsTableCell">` + data.timestamp + `</td>
			<td class="logsTableLevel logsTableCell">` + data.level +`</td>
			<td class="logsTableText logsTableCell">` + data.str + `</td>
		</tr>`
	);
});