#!/bin/bash

if [[ $# != 1 ]]; then
  echo "Usage: repair_stubgen.sh path"
  exit 1
fi

a='def Interval(policy: typing.Any = None) -> typing.Any:
    ...'

b='@typing.overload
def Interval(policy: None = None) -> _Interval_UnknownTypePolicy: ...
@typing.overload
def Interval(policy: FloatTypePolicy) -> _Interval_FloatTypePolicy: ...

@typing.overload
def Interval(policy: IntTypePolicy) -> _Interval_IntTypePolicy: ...

@typing.overload
def Interval(policy: UnknownTypePolicy) -> _Interval_UnknownTypePolicy: ...'

export A="$a"
export B="$b"

perl -0777 -i -pe '
  s/\Q$ENV{"A"}\E/$ENV{"B"}/g;
' "$1"
