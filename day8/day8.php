
<?php


function follow_map($map, $path, $start, $end){
    $lr_table = ["L" => 0, "R" => 1];
    $step_number = 0;
    $path_step_i = 0;
    $current = $start;
    while($current != $end){
    // for ($i = 0; $i < 285; $i++){
        $path_step = $path[$path_step_i];
        $current_choice = $map[$current];
        $current = $current_choice[$lr_table[$path_step]];
        $step_number++;
        $path_step_i++;
        if ($path_step_i >= strlen($path) - 1){
            $path_step_i = 0;
        }
    }
    return $step_number;
}

function get_starts($locations){
    $starts = [];
    foreach($locations as $location => $v){
        if ($location[2] == 'A'){
            $starts[] = $location;
        }
    }
    return $starts;
}

function are_all_ended($locations){
    foreach ($locations as $location){
        if ($location[2] != 'Z'){
            return FALSE;
        }
    }
    return TRUE;
}

function follow_maps($map, $path, $starts){
    $lr_table = ["L" => 0, "R" => 1];
    $step_number = 0;
    $path_step_i = 0;
    $currents = $starts;
    while(true){
    // for ($i = 0; $i < 285; $i++){
        $path_step = $path[$path_step_i];
        for($i =0; $i<count($currents); $i++){
            $current = $currents[$i];
            $current_choice = $map[$current];
            $currents[$i] = $current_choice[$lr_table[$path_step]];
        }
        $step_number++;
        $path_step_i++;
        if ($path_step_i >= strlen($path) - 1){
            $path_step_i = 0;
        }
        if (are_all_ended($currents)){
            echo "$step_number\n";
        }
    }
    return $step_number;
}

function get_cycles($map, $path, $start){
    $lr_table = ["L" => 0, "R" => 1];
    $trace = [];
    $step_number = 0;
    $path_step_i = 0;
    $current = $start;
    $cycle = false;
    $end_states = [];
    while(!$cycle){
    // for ($i = 0; $i < 285; $i++){
        $trace_item = $current . strval($path_step_i);
        if (array_key_exists($trace_item, $trace)){
            $cycle = true;
            break;
        }
        if ($current[2] == 'Z'){
            $end_states[] = $step_number;
        }
        $trace[$trace_item] = $step_number;
        $path_step = $path[$path_step_i];
        $current_choice = $map[$current];
        $current = $current_choice[$lr_table[$path_step]];
        $step_number++;
        $path_step_i++;
        if ($path_step_i >= strlen($path) - 1){
            $path_step_i = 0;
        }
    }
    return [$trace[$trace_item], $step_number, $end_states];
}

$map = [];
$fp = @fopen("./input.txt", "r");
if ($fp) {
    $path = fgets($fp, 4096);
    fgets($fp, 4096);
    while (($buffer = fgets($fp, 4096)) !== false) {
        $expl = explode("=", $buffer);
        $paths = explode(",", $expl[1]);
        $left = str_replace("(", "", $paths[0]);
        $left = trim($left);
        $right = str_replace(")", "", $paths[1]);
        $right = trim($right);
        $map[trim($expl[0])] = [$left, $right];
    }
    if (!feof($fp)) {
        echo "Error: unexpected fgets() fail\n";
    }
    fclose($fp);

   // print_r($map);

    $start = "AAA";
    $end = "ZZZ";
    $pu1 = follow_map($map, $path, $start, $end);
    echo "Puzzle 1 solution: $pu1\n";
    $starts = get_starts($map);
    $cycles = [];
    foreach ($starts as $st){
        $cycles[] = get_cycles($map, $path, $st);
    }
    //print(follow_maps($map, $path, [$starts[0]]));
    $lcm = $cycles[0][2][0];  // Apparently only on final state per cycle; and offset in the cycle is equal to the offset to get in the cycle. So PCM works without thinking.
    for ($i = 1; $i < count($cycles); $i++){
        $lcm = gmp_lcm($lcm, $cycles[$i][2][0]);
    }
    echo "Puzzle 2 solution: $lcm\n";
    
}
?>
