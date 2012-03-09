
Handle<Value> BitVector::New( const Arguments& args ) {
  HandleScope scope;

  MyObject* obj = new MyObject();
  obj->Wrap(args.This());

  return args.This();
}

