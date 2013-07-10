var data;
var table;

function startTest() {
    google.load('visualization', '1', {packages:['table']});
    google.setOnLoadCallback(runTest);
}

function runTest(){
    drawTable();
    initConnection();
}

function wolfPropertyCallback(receivedProperty){
      //alert(receivedProperty.toString());
      addRow(receivedProperty);
}

function drawTable() {
        
        
	data = new google.visualization.DataTable();
	data.addColumn('number', 'Identifier');
	data.addColumn('string', 'Name');
	data.addColumn('string', 'Description');
        data.addColumn('string', 'Type');
	data.addColumn('string', 'Value');
	data.addColumn('number', 'Timestamp Seconds');
	data.addColumn('number', 'Timestamp microSeconds');

	table = new google.visualization.Table(document.getElementById('table_div'));
	table.draw(data, {showRowNumber: false});

}


function addRow(p) {
	data.addRow([p.id, p.name, p.description,p.getValueTypeString(), p.value.toString(), p.secTimestamp, p.uSecTimestamp]);
	table.draw(data, {showRowNumber: false});
}