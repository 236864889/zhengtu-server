<?php
// 用法：php epay_sign.php --key "xxx" --data "a=1&b=2"

$options = getopt('', ['key:', 'data:']);
$key = $options['key'] ?? '';
$dataRaw = $options['data'] ?? '';

if ($key === '' || $dataRaw === '') {
    fwrite(STDERR, "Usage: php epay_sign.php --key \"EPAY_KEY\" --data \"k1=v1&k2=v2\"\n");
    exit(1);
}

parse_str($dataRaw, $arr);

unset($arr['sign'], $arr['sign_type']);
foreach ($arr as $k => $v) {
    if ($v === '' || $v === null) unset($arr[$k]);
}
ksort($arr);

$signStr = urldecode(http_build_query($arr)) . $key;
$sign = strtolower(md5($signStr));

echo "sign_str={$signStr}\n";
echo "sign={$sign}\n";
