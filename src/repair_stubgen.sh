if [[ $# != 1 ]]
then
  echo "using repair_stubgen.sh path"
  exit 1
fi
export a='def Interval(policy: typing.Any = None) -> typing.Any:
              ...'
export b='@typing.overload
def Interval(policy: None = None) -> _Interval_UnknownTypePolicy: ...
@typing.overload
def Interval(policy: FloatTypePolicy) -> _Interval_FloatTypePolicy: ...

@typing.overload
def Interval(policy: IntTypePolicy) -> _Interval_IntTypePolicy: ...

@typing.overload
def Interval(policy: UnknownTypePolicy) -> _Interval_UnknownTypePolicy: ...'
sed -i "s|${a}|${b}|" "$1"