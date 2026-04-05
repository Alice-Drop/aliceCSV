// aliceCSV v1.0.0 JavaScript 部分

function parseLine(line, delimiter=","){
	/*
	把CSV文件指定的一行转换为数组
	*/
	
	let output_list = [];
	if (line.indexOf('"')===-1){ //如果这一行没有引号，肯定没有引文，直接拆分
		output_list = line.split(delimiter);
	}else{
		let division_list = line.split(delimiter); // 先把整行通过逗号分割，可能会有不应分割的（用引号括起来的部分）
        let quoting = false;
        let cache = ""
        for (let item of division_list){
            if (quoting){
                if (item.slice(-1) ==='"'){
					cache += item;
					cache = cache.slice(1,- 1);  // 此时cache开头和结尾是双引号，需要去掉
					cache = cache.replace('""', '"');  // 根据RFC 4180定义第7条，引文里面的双引号要变成两个，所以解读的时候要变回一个
					output_list.push(cache);
					cache = ""
					quoting = false
                }else {
					cache += item
					cache += delimiter  // 继续补回split去掉的分隔符
				}
            }else { //不是引文状态：
				if (item.length === 0){
					output_list.push("")

				}else if (item[0]==='"'){ //如果第一项是引号，说明开始了一个引文  *****
					if (item.slice(-1)==='"') { // 如果最后一项是 " ，那么就是说在引文里面没有逗号，引用在这个item里面就开始并结束，所以读到的这个item就是列表的一个列
						item = item.slice(1,-1)  // 需要去掉开头和结尾的引号，提取出真正的内容
						item = item.replace('""','"')
						output_list.push(item)
					}else { // 最后一项不是" ，那么就是说开始了一段引文，引文里有逗号所以拆成了几段，开quoting开始分析后面的  *****
						quoting = true
						cache += item
						cache += delimiter // 也就是说这里还是引文里面，逗号被split用掉了，所以要补上一个。
					}
				}else { // 如果第一项不是引号，也就是说包含了 123"cde"f 之类的乱七八糟的格式，或者是不含引号的最普通的项，都是直接加到列表
					output_list.push(item)
				}
			}
        }
	}
	return output_list

}


function parseCSV(csvText,delimiter=","){
	// 传入csv文件的文本，返回解析出的表格

	let data = csvText.split(/\r\n|\n/);  // 把文本每行提取出来
	for (let i=0;i< data.length;i++) {
		data[i] = parseLine(data[i], delimiter);
	}

	return data

}

function fixLineLength(csv_sheet){
	/*
		根据RFC 4180 中csv文件的定义的第4条，每行应该含有相同数量的字段。因此对于数量对不上的要进行修复。
		输入一个二维数组表示的表格，输出检查并修复完成的版本。
	 */

	let maxLength = 0;
	for (let row of csv_sheet){  //遍历每一行，确定最长的一行
		if (row.length > maxLength){
			maxLength = row.length;
		}
	}
	//console.log(`最大长度为${maxLength}`);

	csv_sheet.map(function (row,index){ // 让每一行都等于最长的长度
		//console.log(`第${index}行，max${maxLength},len${row.length},max-len = ${maxLength-row.length}`);

		while (row.length < maxLength){
			row.push("") // 给数组加上一个空的内容
			console.log(`给这一行(${row})添加了1次，此时maxLen${maxLength} - len${row.length} = ${maxLength-row.length}`);
		}
		csv_sheet[index] = row

		//console.log("搞定第",index,"行")
	})
	return csv_sheet

}

function writeCSV(table,delimiter=",", sheet_encoding="UTF-8",line_break = "\n"){
	// 把输入的二维列表输出为一个blob形式的csv文件

	let csv_text = ""
	for (let row of table) {
		for (let i = 0; i < row.length; i++){
			let col = row[i]
			if (col.includes('"')) { // 判断是否需要使用引文。
				csv_text += '"';   // 如果这一个字段有引号，就用双引号括起来
				csv_text += col.replace('"','""');
				csv_text += '"';

			}else if (col.includes(",") || col.includes('\n') || col.includes(delimiter)) {
				csv_text += '"';  // 如果这一项里有逗号或是换行符，就给它加上双引号。
				csv_text += col;
				csv_text += '"';
			}else{  // 如果上述情况都没有，那就是正常情况，直接字段写上去
				csv_text += col;
			}

			if (i !== row.length -1) {
				csv_text += delimiter;
			}
		}
		csv_text += line_break; // 换行
	}

	return new Blob([csv_text], {type: `text/plain;charset=${sheet_encoding}`})
}

function fixCSV(csvFile, original_delimiter=",", target_delimiter = ",", original_encoding="UTF-8",target_encoding="UTF-8",target_line_break="\n",fix_length=true){
	// 输入一个blob形式的csv文件和相关信息，返回一个Promise，解析即可得到结果的blob

	return new Promise((resolve,reject)=>{
		let reader = new FileReader();
		reader.onloadend = function(){
			let table = parseCSV(reader.result, original_delimiter);
			if (fix_length){
				table = fixLineLength(table);
			}
			let output_file = writeCSV(table,target_delimiter,original_encoding,)

			resolve(output_file);
		}
		reader.onerror = function () {
			reject("警告！无法读取指定的CSV文件！")
		}
		reader.readAsText(csvFile)

	})
}

	