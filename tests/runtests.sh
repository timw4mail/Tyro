for i in tests/*Test
do
    if test -f $i
    then
        ./$i
    fi
done

echo ""